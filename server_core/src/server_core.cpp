#include "evolve/server_core.hpp"

#include "evolve/resource_store.hpp"
#include "evolve/server_logic.hpp"

#include <civetweb.h>

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/error.h>
#include <mbedtls/oid.h>
#include <mbedtls/pk.h>
#include <mbedtls/rsa.h>
#include <mbedtls/x509_crt.h>

#include <array>
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace evolve::server {
namespace {

using CertPtr = std::unique_ptr<mbedtls_x509_crt, decltype(&mbedtls_x509_crt_free)>;
using PkPtr = std::unique_ptr<mbedtls_pk_context, decltype(&mbedtls_pk_free)>;

std::string MbedTlsError(int err) {
  char buf[256];
  mbedtls_strerror(err, buf, sizeof(buf));
  return std::string(buf);
}

std::string BuildListeningPortsOption(const std::string& bind_address, std::uint16_t port) {
  return bind_address + ":" + std::to_string(port) + "s";
}

std::string GetContentType(const ResponsePlan& plan) {
  switch (plan.kind) {
    case ResponseKind::kTwoK:
      return "application/json";
    case ResponseKind::kMultiplayer:
      return "application/json; charset=utf-8";
    case ResponseKind::kHotfix:
      return "application/octet-stream";
    case ResponseKind::kDefault:
    default:
      return "application/json";
  }
}

std::string ReadRequestBody(mg_connection* connection, long long content_length) {
  if (content_length <= 0) {
    return {};
  }

  std::string body;
  body.resize(static_cast<std::size_t>(content_length));

  long long total_read = 0;
  while (total_read < content_length) {
    const auto chunk_size = static_cast<size_t>(content_length - total_read);
    const int read_now = mg_read(connection, body.data() + total_read, chunk_size);
    if (read_now <= 0) {
      break;
    }
    total_read += read_now;
  }

  body.resize(static_cast<std::size_t>(total_read));
  return body;
}

std::optional<std::uint16_t> ProbePort(const std::string& bind_address, std::uint16_t port) {
#ifdef _WIN32
  static std::atomic<bool> winsock_ready{false};
  if (!winsock_ready.load(std::memory_order_acquire)) {
    WSADATA wsa_data{};
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
      return std::nullopt;
    }
    winsock_ready.store(true, std::memory_order_release);
  }
#endif

  const int sock = ::socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return std::nullopt;
  }

  int reuse = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  if (bind_address == "0.0.0.0") {
    address.sin_addr.s_addr = htonl(INADDR_ANY);
  } else if (inet_pton(AF_INET, bind_address.c_str(), &address.sin_addr) != 1) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    return std::nullopt;
  }

  if (::bind(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    return std::nullopt;
  }

  sockaddr_in assigned{};
  socklen_t assigned_length = sizeof(assigned);
  if (::getsockname(sock, reinterpret_cast<sockaddr*>(&assigned), &assigned_length) != 0) {
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    return std::nullopt;
  }

#ifdef _WIN32
  closesocket(sock);
#else
  close(sock);
#endif

  return ntohs(assigned.sin_port);
}

std::optional<std::uint16_t> SelectPort(const ServerConfig& config) {
  if (const auto preferred = ProbePort(config.bind_address, config.preferred_port)) {
    return preferred;
  }
  return ProbePort(config.bind_address, 0);
}

bool LoadCaCert(const std::filesystem::path& path, mbedtls_x509_crt& out) {
  return mbedtls_x509_crt_parse_file(&out, path.string().c_str()) == 0;
}

bool LoadCaKey(const std::filesystem::path& path, mbedtls_pk_context& out) {
  return mbedtls_pk_parse_keyfile(&out, path.string().c_str(), nullptr) == 0;
}

std::vector<unsigned char> BuildSubjectAltNameDer(const std::string& hostname) {
  size_t host_len = hostname.size();
  std::vector<unsigned char> der;
  der.reserve(4 + host_len);
  der.push_back(0x30);
  der.push_back(static_cast<unsigned char>(2 + host_len));
  der.push_back(0x82);
  der.push_back(static_cast<unsigned char>(host_len));
  der.insert(der.end(), hostname.begin(), hostname.end());
  return der;
}

std::optional<std::filesystem::path> GenerateServerPem(const ServerConfig& config,
                                                       std::string* error_out) {
  mbedtls_x509_crt ca_cert;
  mbedtls_x509_crt_init(&ca_cert);
  if (!LoadCaCert(config.ca_certificate_path, ca_cert)) {
    mbedtls_x509_crt_free(&ca_cert);
    if (error_out) {
      *error_out = "failed to load CA certificate";
    }
    return std::nullopt;
  }

  mbedtls_pk_context ca_key;
  mbedtls_pk_init(&ca_key);
  if (!LoadCaKey(config.ca_private_key_path, ca_key)) {
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_pk_free(&ca_key);
    if (error_out) {
      *error_out = "failed to load CA private key";
    }
    return std::nullopt;
  }

  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  const char* pers = "evolve_server_keygen";
  int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                   reinterpret_cast<const unsigned char*>(pers), strlen(pers));
  if (ret != 0) {
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_pk_free(&ca_key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    if (error_out) {
      *error_out = "failed to seed RNG: " + MbedTlsError(ret);
    }
    return std::nullopt;
  }

  mbedtls_pk_context server_key;
  mbedtls_pk_init(&server_key);

  ret = mbedtls_pk_setup(&server_key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
  if (ret != 0) {
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_pk_free(&ca_key);
    mbedtls_pk_free(&server_key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    if (error_out) {
      *error_out = "failed to setup server key: " + MbedTlsError(ret);
    }
    return std::nullopt;
  }

  ret = mbedtls_rsa_gen_key(mbedtls_pk_rsa(server_key), mbedtls_ctr_drbg_random, &ctr_drbg,
                             2048, 65537);
  if (ret != 0) {
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_pk_free(&ca_key);
    mbedtls_pk_free(&server_key);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    if (error_out) {
      *error_out = "failed to generate server RSA key: " + MbedTlsError(ret);
    }
    return std::nullopt;
  }

  mbedtls_x509write_crt crt;
  mbedtls_x509write_crt_init(&crt);

  mbedtls_x509write_crt_set_version(&crt, MBEDTLS_X509_CRT_VERSION_3);
  mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);

  mbedtls_mpi serial;
  mbedtls_mpi_init(&serial);
  auto serial_str = std::to_string(std::time(nullptr));
  mbedtls_mpi_read_string(&serial, 10, serial_str.c_str());
  mbedtls_x509write_crt_set_serial(&crt, &serial);
  mbedtls_mpi_free(&serial);

  char not_before[16];
  char not_after[16];
  time_t now = std::time(nullptr);
  strftime(not_before, sizeof(not_before), "%Y%m%d%H%M%S", gmtime(&now));
  time_t later = now + 60L * 60L * 24L * 365L * 5L;
  strftime(not_after, sizeof(not_after), "%Y%m%d%H%M%S", gmtime(&later));
  mbedtls_x509write_crt_set_validity(&crt, not_before, not_after);

  mbedtls_x509write_crt_set_subject_name(&crt,
                                         ("CN=" + config.certificate_host_pattern).c_str());

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  char issuer_name[512];
  mbedtls_x509_dn_gets(issuer_name, sizeof(issuer_name), &ca_cert.subject);
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
  mbedtls_x509write_crt_set_issuer_name(&crt, issuer_name);

  mbedtls_x509write_crt_set_subject_key(&crt, &server_key);
  mbedtls_x509write_crt_set_issuer_key(&crt, &ca_key);

  mbedtls_x509write_crt_set_basic_constraints(&crt, 0, -1);
  mbedtls_x509write_crt_set_key_usage(&crt,
                                       MBEDTLS_X509_KU_DIGITAL_SIGNATURE |
                                           MBEDTLS_X509_KU_KEY_ENCIPHERMENT);
  mbedtls_x509write_crt_set_ns_cert_type(&crt, MBEDTLS_X509_NS_CERT_TYPE_SSL_SERVER);

  static const unsigned char ext_key_usage_val[] = {
      0x30, 0x08, 0x06, 0x06, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01};
  mbedtls_x509write_crt_set_extension(&crt, MBEDTLS_OID_EXTENDED_KEY_USAGE,
                                       MBEDTLS_OID_SIZE(MBEDTLS_OID_EXTENDED_KEY_USAGE), 0,
                                       ext_key_usage_val, sizeof(ext_key_usage_val));

  auto san_der = BuildSubjectAltNameDer(config.certificate_host_pattern);
  mbedtls_x509write_crt_set_extension(&crt, MBEDTLS_OID_SUBJECT_ALT_NAME,
                                       MBEDTLS_OID_SIZE(MBEDTLS_OID_SUBJECT_ALT_NAME), 0,
                                       san_der.data(), san_der.size());

  mbedtls_x509write_crt_set_subject_key_identifier(&crt);
  mbedtls_x509write_crt_set_authority_key_identifier(&crt);

  unsigned char cert_pem[8192];
  ret = mbedtls_x509write_crt_pem(&crt, cert_pem, sizeof(cert_pem), mbedtls_ctr_drbg_random,
                                   &ctr_drbg);
  if (ret < 0) {
    mbedtls_x509write_crt_free(&crt);
    mbedtls_pk_free(&server_key);
    mbedtls_pk_free(&ca_key);
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    if (error_out) {
      *error_out = "failed to write server certificate PEM: " + MbedTlsError(ret);
    }
    return std::nullopt;
  }

  unsigned char key_pem[4096];
  ret = mbedtls_pk_write_key_pem(&server_key, key_pem, sizeof(key_pem));
  if (ret < 0) {
    mbedtls_x509write_crt_free(&crt);
    mbedtls_pk_free(&server_key);
    mbedtls_pk_free(&ca_key);
    mbedtls_x509_crt_free(&ca_cert);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    if (error_out) {
      *error_out = "failed to write server key PEM: " + MbedTlsError(ret);
    }
    return std::nullopt;
  }

  mbedtls_x509write_crt_free(&crt);
  mbedtls_pk_free(&server_key);
  mbedtls_pk_free(&ca_key);
  mbedtls_x509_crt_free(&ca_cert);
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);

  const auto cache_dir = std::filesystem::current_path() / ".cache" / "server_core";
  std::filesystem::create_directories(cache_dir);
  const auto pem_path = cache_dir / "runtime-server.pem";

  std::ofstream out(pem_path, std::ios::binary);
  if (!out) {
    if (error_out) {
      *error_out = "failed to open PEM output file";
    }
    return std::nullopt;
  }
  out.write(reinterpret_cast<const char*>(cert_pem), strlen(reinterpret_cast<const char*>(cert_pem)));
  out.write(reinterpret_cast<const char*>(key_pem), strlen(reinterpret_cast<const char*>(key_pem)));
  out.close();
  if (!out.good()) {
    if (error_out) {
      *error_out = "failed to write PEM file";
    }
    return std::nullopt;
  }

  return pem_path;
}

}  // namespace

struct ServerCore::Impl {
  explicit Impl(ServerConfig in_config)
      : config(std::move(in_config)), resources(config.asset_root) {}

  ServerConfig config;
  ResourceStore resources;
  mg_context* context = nullptr;
  std::uint16_t bound_port = 0;
  std::string last_error;
  bool ready = false;
  std::filesystem::path generated_pem_path;

  static int HandleRequest(mg_connection* connection, void* user_data) {
    return static_cast<Impl*>(user_data)->HandleRequest(connection);
  }

  int HandleRequest(mg_connection* connection) {
    const auto* request_info = mg_get_request_info(connection);
    const std::string path = request_info->local_uri ? request_info->local_uri : request_info->request_uri;
    const std::string request_body = ReadRequestBody(connection, request_info->content_length);

    const auto plan = BuildResponsePlan(path, request_body);
    if (!plan.has_value()) {
      static constexpr std::string_view default_body = R"({"message":"OK"})";
      mg_send_http_ok(connection, "application/json", default_body.size());
      mg_write(connection, default_body.data(), default_body.size());
      return 200;
    }

    if (plan->kind == ResponseKind::kHotfix) {
      const auto asset = resources.ReadBinary(plan->asset_name);
      if (!asset.has_value()) {
        mg_send_http_error(connection, 404, "asset not found: %s", plan->asset_name.c_str());
        return 404;
      }
      mg_send_http_ok(connection, GetContentType(*plan).c_str(), asset->size());
      mg_write(connection, asset->data(), asset->size());
      return 200;
    }

    const auto asset = resources.ReadText(plan->asset_name);
    if (!asset.has_value()) {
      mg_send_http_error(connection, 404, "asset not found: %s", plan->asset_name.c_str());
      return 404;
    }

    const auto response_body = ApplyResponseTransforms(
        *plan, *asset, request_body, config.steam_id, static_cast<std::int64_t>(std::time(nullptr)));

    mg_send_http_ok(connection, GetContentType(*plan).c_str(), response_body.size());
    mg_write(connection, response_body.data(), response_body.size());
    return 200;
  }
};

ServerCore::ServerCore(ServerConfig config) : impl_(std::make_unique<Impl>(std::move(config))) {}

ServerCore::~ServerCore() {
  Stop();
}

bool ServerCore::Start() {
  if (impl_->ready) {
    return true;
  }

  const auto selected_port = SelectPort(impl_->config);
  if (!selected_port.has_value()) {
    impl_->last_error = "failed to reserve a listening port";
    return false;
  }

  impl_->bound_port = *selected_port;
  const auto pem_path = GenerateServerPem(impl_->config, &impl_->last_error);
  if (!pem_path.has_value()) {
    return false;
  }
  impl_->generated_pem_path = *pem_path;

  const auto listening_ports =
      BuildListeningPortsOption(impl_->config.bind_address, impl_->bound_port);
  const auto num_threads = std::to_string(impl_->config.num_threads);

  std::vector<std::string> option_values = {
      "listening_ports",
      listening_ports,
      "ssl_certificate",
      impl_->generated_pem_path.string(),
      "num_threads",
      num_threads,
      "enable_keep_alive",
      "yes",
  };

  std::vector<const char*> options;
  options.reserve(option_values.size() + 1);
  for (const auto& value : option_values) {
    options.push_back(value.c_str());
  }
  options.push_back(nullptr);

  mg_init_library(0);
  impl_->context = mg_start(nullptr, impl_.get(), options.data());
  if (!impl_->context) {
    impl_->last_error = "mg_start failed";
    return false;
  }

  mg_set_request_handler(impl_->context, "/", &Impl::HandleRequest, impl_.get());
  impl_->ready = true;
  impl_->last_error.clear();
  return true;
}

void ServerCore::Stop() {
  if (!impl_->context) {
    impl_->ready = false;
    return;
  }

  mg_stop(impl_->context);
  impl_->context = nullptr;
  impl_->ready = false;
  impl_->bound_port = 0;
}

bool ServerCore::IsReady() const {
  return impl_->ready;
}

std::uint16_t ServerCore::GetBoundPort() const {
  return impl_->bound_port;
}

std::string ServerCore::GetRedirectHost() const {
  if (impl_->config.bind_address.empty() || impl_->config.bind_address == "0.0.0.0") {
    return "127.0.0.1";
  }
  return impl_->config.bind_address;
}

std::string ServerCore::GetLastError() const {
  return impl_->last_error;
}

}  // namespace evolve::server
