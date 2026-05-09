#include "evolve/resource_store.hpp"

#include <fstream>
#include <iterator>

namespace evolve::server {

ResourceStore::ResourceStore(std::filesystem::path asset_root)
    : asset_root_(std::move(asset_root)) {}

std::filesystem::path ResourceStore::Resolve(std::string_view asset_name) const {
  return asset_root_ / std::filesystem::path(asset_name);
}

std::optional<std::string> ResourceStore::ReadText(std::string_view asset_name) const {
  std::ifstream input(Resolve(asset_name), std::ios::in | std::ios::binary);
  if (!input) {
    return std::nullopt;
  }

  return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

std::optional<std::vector<unsigned char>> ResourceStore::ReadBinary(
    std::string_view asset_name) const {
  std::ifstream input(Resolve(asset_name), std::ios::in | std::ios::binary);
  if (!input) {
    return std::nullopt;
  }

  std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(input)),
                                   std::istreambuf_iterator<char>());
  return bytes;
}

}  // namespace evolve::server
