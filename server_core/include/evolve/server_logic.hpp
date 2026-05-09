#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace evolve::server {

enum class ResponseKind {
  kDefault,
  kTwoK,
  kMultiplayer,
  kHotfix,
};

struct ResponsePlan {
  ResponseKind kind = ResponseKind::kDefault;
  std::string asset_name;
  bool replace_app_context = false;
  bool replace_server_time = false;
  bool replace_created_on = false;
  bool replace_owner_steam_id = false;
};

std::optional<ResponsePlan> BuildResponsePlan(std::string_view path,
                                              std::string_view request_body);

std::string ApplyResponseTransforms(const ResponsePlan& plan,
                                    std::string_view template_body,
                                    std::string_view request_body,
                                    std::string_view steam_id,
                                    std::int64_t unix_time);

}  // namespace evolve::server
