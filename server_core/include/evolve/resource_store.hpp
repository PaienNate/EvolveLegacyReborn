#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace evolve::server {

class ResourceStore {
 public:
  explicit ResourceStore(std::filesystem::path asset_root);

  std::filesystem::path Resolve(std::string_view asset_name) const;
  std::optional<std::string> ReadText(std::string_view asset_name) const;
  std::optional<std::vector<unsigned char>> ReadBinary(std::string_view asset_name) const;

 private:
  std::filesystem::path asset_root_;
};

}  // namespace evolve::server
