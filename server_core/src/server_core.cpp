#include "evolve/server_core.hpp"

#include "evolve/resource_store.hpp"
#include "evolve/server_logic.hpp"

#include <civetweb.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

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

using X509Ptr = std::unique_ptr<X509, decltype(&X509_free)>;
using EvpKeyPtr = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;
using BioPtr = std::unique_ptr<BIO, decltype(&BIO_free)>;
using BignumPtr = std::unique_ptr<BIGNUM, decltype(&BN_free)>;
using X509ExtPtr = std::unique_ptr<X509_EXTENSION, decltype(&X509_EXTENSION_free)>;

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

X509Ptr LoadCertificate(const std::filesystem::path& path) {
  auto bio = BioPtr(BIO_new_file(path.string().c_str(), "r"), BIO_free);
  if (!bio) {
    return {nullptr, X509_free};
  }
  return X509Ptr(PEM_read_bio_X509(bio.get(), nullptr, nullptr, nullptr), X509_free);
}

EvpKeyPtr LoadPrivateKey(const std::filesystem::path& path) {
  auto bio = BioPtr(BIO_new_file(path.string().c_str(), "r"), BIO_free);
  if (!bio) {
    return {nullptr, EVP_PKEY_free};
  }
  return EvpKeyPtr(PEM_read_bio_PrivateKey(bio.get(), nullptr, nullptr, nullptr), EVP_PKEY_free);
}

bool AddExtension(X509* certificate,
                  X509* issuer,
                  int nid,
                  const std::string& value) {
  X509V3_CTX context{};
  X509V3_set_ctx(&context, issuer, certificate, nullptr, nullptr, 0);
  auto extension = X509ExtPtr(
      X509V3_EXT_conf_nid(nullptr, &context, nid, const_cast<char*>(value.c_str())),
      X509_EXTENSION_free);
  if (!extension) {
    return false;
  }
  return X509_add_ext(certificate, extension.get(), -1) == 1;
}

std::optional<std::filesystem::path> GenerateServerPem(const ServerConfig& config,
                                                       std::string* error_out) {
  auto issuer_certificate = LoadCertificate(config.ca_certificate_path);
  auto issuer_key = LoadPrivateKey(config.ca_private_key_path);
  if (!issuer_certificate || !issuer_key) {
    if (error_out) {
      *error_out = "failed to load CA certificate or private key";
    }
    return std::nullopt;
  }

  auto server_key = EvpKeyPtr(EVP_PKEY_new(), EVP_PKEY_free);
  if (!server_key) {
    if (error_out) {
      *error_out = "failed to allocate server key";
    }
    return std::nullopt;
  }

  auto rsa = RSA_new();
  auto exponent = BignumPtr(BN_new(), BN_free);
  if (!rsa || !exponent || BN_set_word(exponent.get(), RSA_F4) != 1 ||
      RSA_generate_key_ex(rsa, 2048, exponent.get(), nullptr) != 1 ||
      EVP_PKEY_assign_RSA(server_key.get(), rsa) != 1) {
    RSA_free(rsa);
    if (error_out) {
      *error_out = "failed to generate server RSA key";
    }
    return std::nullopt;
  }

  auto server_certificate = X509Ptr(X509_new(), X509_free);
  if (!server_certificate) {
    if (error_out) {
      *error_out = "failed to allocate server certificate";
    }
    return std::nullopt;
  }

  X509_set_version(server_certificate.get(), 2);
  ASN1_INTEGER_set(X509_get_serialNumber(server_certificate.get()),
                   static_cast<long>(std::time(nullptr)));
  X509_gmtime_adj(X509_get_notBefore(server_certificate.get()), 0);
  X509_gmtime_adj(X509_get_notAfter(server_certificate.get()), 60L * 60L * 24L * 365L * 5L);
  X509_set_pubkey(server_certificate.get(), server_key.get());
  X509_set_issuer_name(server_certificate.get(), X509_get_subject_name(issuer_certificate.get()));

  auto* subject = X509_get_subject_name(server_certificate.get());
  X509_NAME_add_entry_by_txt(
      subject, "CN", MBSTRING_ASC,
      reinterpret_cast<const unsigned char*>(config.certificate_host_pattern.c_str()), -1, -1, 0);

  if (!AddExtension(server_certificate.get(), issuer_certificate.get(), NID_basic_constraints,
                    "CA:FALSE") ||
      !AddExtension(server_certificate.get(), issuer_certificate.get(), NID_key_usage,
                    "digitalSignature,keyEncipherment") ||
      !AddExtension(server_certificate.get(), issuer_certificate.get(), NID_ext_key_usage,
                    "serverAuth") ||
      !AddExtension(server_certificate.get(), issuer_certificate.get(), NID_subject_alt_name,
                    "DNS:" + config.certificate_host_pattern) ||
      X509_sign(server_certificate.get(), issuer_key.get(), EVP_sha256()) <= 0) {
    if (error_out) {
      *error_out = "failed to sign runtime TLS certificate";
    }
    return std::nullopt;
  }

  const auto cache_dir = std::filesystem::current_path() / ".cache" / "server_core";
  std::filesystem::create_directories(cache_dir);
  const auto pem_path = cache_dir / "runtime-server.pem";

  auto output = BioPtr(BIO_new_file(pem_path.string().c_str(), "w"), BIO_free);
  if (!output || PEM_write_bio_X509(output.get(), server_certificate.get()) != 1 ||
      PEM_write_bio_PrivateKey(output.get(), server_key.get(), nullptr, nullptr, 0, nullptr,
                               nullptr) != 1) {
    if (error_out) {
      *error_out = "failed to write runtime TLS certificate";
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
