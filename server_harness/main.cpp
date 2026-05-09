#include "evolve/server_core.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

namespace {

evolve::server::ServerConfig BuildConfig(int argc, char** argv) {
  evolve::server::ServerConfig config;
  config.asset_root = "EvolveServerEmulator/EvolveCrack";
  config.ca_certificate_path = "EvolveServerEmulator/certs/mitmproxy-ca-cert.pem";
  config.ca_private_key_path = "EvolveServerEmulator/certs/mitmproxy-ca.pem";
  if (argc > 1) {
    config.asset_root = argv[1];
  }
  if (argc > 2) {
    config.preferred_port = static_cast<std::uint16_t>(std::stoi(argv[2]));
  }
  if (argc > 3) {
    config.bind_address = argv[3];
  }
  if (argc > 4) {
    config.ca_certificate_path = argv[4];
  }
  if (argc > 5) {
    config.ca_private_key_path = argv[5];
  }
  return config;
}

}  // namespace

int main(int argc, char** argv) {
  try {
    auto config = BuildConfig(argc, argv);
    int hold_seconds = 2;
    if (argc > 6) {
      hold_seconds = std::stoi(argv[6]);
    }
    evolve::server::ServerCore server(std::move(config));

    if (!server.Start()) {
      std::cerr << "failed to start server_harness: " << server.GetLastError() << '\n';
      return 1;
    }

    std::cout << "server_harness listening on https://" << server.GetRedirectHost() << ':'
              << server.GetBoundPort() << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(hold_seconds));
    server.Stop();
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return 1;
  }
}
