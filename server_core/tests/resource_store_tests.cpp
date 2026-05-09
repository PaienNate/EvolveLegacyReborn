#include "evolve/resource_store.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using evolve::server::ResourceStore;

void TestResourceStoreReadsTextAsset() {
  const auto root = std::filesystem::temp_directory_path() / "evolve-resource-store-text";
  std::filesystem::create_directories(root);
  std::ofstream(root / "sample.json") << "{\"ok\":true}";

  ResourceStore store(root);
  const auto content = store.ReadText("sample.json");
  if (!content.has_value() || *content != "{\"ok\":true}") {
    throw std::runtime_error("resource store should read text assets");
  }
}

void TestResourceStoreReadsBinaryAsset() {
  const auto root = std::filesystem::temp_directory_path() / "evolve-resource-store-binary";
  std::filesystem::create_directories(root);
  {
    std::ofstream output(root / "sample.bin", std::ios::binary);
    const unsigned char bytes[] = {0x01, 0x02, 0x03};
    output.write(reinterpret_cast<const char*>(bytes), sizeof(bytes));
  }

  ResourceStore store(root);
  const auto content = store.ReadBinary("sample.bin");
  if (!content.has_value() || content->size() != 3 || (*content)[2] != 0x03) {
    throw std::runtime_error("resource store should read binary assets");
  }
}

void TestResourceStoreReturnsEmptyForMissingAsset() {
  const auto root = std::filesystem::temp_directory_path() / "evolve-resource-store-missing";
  std::filesystem::create_directories(root);

  ResourceStore store(root);
  if (store.ReadText("missing.json").has_value()) {
    throw std::runtime_error("resource store should not fabricate missing text assets");
  }
}

}  // namespace

void RunResourceStoreTests() {
  TestResourceStoreReadsTextAsset();
  TestResourceStoreReadsBinaryAsset();
  TestResourceStoreReturnsEmptyForMissingAsset();
}
