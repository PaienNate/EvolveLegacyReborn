#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

namespace evolve::server {

struct ServerConfig {
  std::string bind_address = "127.0.0.1";
  std::uint16_t preferred_port = 2000;
  std::filesystem::path asset_root = "assets/EvolveCrack";
  std::filesystem::path ca_certificate_path = "assets/certs/mitmproxy-ca-cert.pem";
  std::filesystem::path ca_private_key_path = "assets/certs/mitmproxy-ca.pem";
  std::string certificate_host_pattern = "*.my.2k.com";
  std::string steam_id = "76561101839859666";
  unsigned int num_threads = 4;
};

class ServerCore {
 public:
  explicit ServerCore(ServerConfig config);
  ~ServerCore();

  ServerCore(const ServerCore&) = delete;
  ServerCore& operator=(const ServerCore&) = delete;

  bool Start();
  void Stop();

  bool IsReady() const;
  std::uint16_t GetBoundPort() const;
  std::string GetRedirectHost() const;
  std::string GetLastError() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace evolve::server
