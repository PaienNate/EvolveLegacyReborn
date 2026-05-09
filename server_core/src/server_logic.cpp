#include "evolve/server_logic.hpp"

#include <regex>
#include <string>
#include <unordered_map>

namespace evolve::server {
namespace {

using ResponseMap = std::unordered_map<std::string, ResponsePlan>;

const ResponseMap& TwoKRoutes() {
  static const ResponseMap routes = {
      {"doorman/1", {ResponseKind::kTwoK, "doorman.json", true, false, false, false}},
      {"telemetry/1",
       {ResponseKind::kTwoK, "telemetryconfiggenerate.json", true, true, true, false}},
      {"sessions/1", {ResponseKind::kTwoK, "sessions.json", true, true, true, false}},
      {"news/1", {ResponseKind::kTwoK, "news.json", true, true, true, false}},
      {"storage/1", {ResponseKind::kTwoK, "iteminsert.json", true, true, true, false}},
      {"sso/1", {ResponseKind::kTwoK, "applogon.json", true, true, true, false}},
      {"stats/1", {ResponseKind::kTwoK, "stats.json", true, true, true, false}},
      {"content/1", {ResponseKind::kTwoK, "stringsget.json", true, true, true, false}},
  };

  return routes;
}

const ResponseMap& MultiplayerRoutes() {
  static const ResponseMap routes = {
      {"auth.php", {ResponseKind::kMultiplayer, "SteamLogin.json", false, false, false, false}},
      {"requestserver.php",
       {ResponseKind::kMultiplayer, "requestserver.json", false, false, false, false}},
  };

  return routes;
}

const ResponseMap& HotfixRoutes() {
  static const ResponseMap routes = {
      {"Production/252019.252019.0/build_config_signed.json",
       {ResponseKind::kHotfix, "build_config_signed0.json", false, false, false, false}},
      {"Production/252019.252019/micro_patch_version_signed.json",
       {ResponseKind::kHotfix, "micro_patch_version_signed.json", false, false, false, false}},
      {"Production/252019.252019/micro_patch_252019.252019_31.pak",
       {ResponseKind::kHotfix, "micro_patch_252019.252019_31.pak", false, false, false, false}},
      {"Production/252019.252019.31/build_config_signed.json",
       {ResponseKind::kHotfix, "build_config_signed_31.json", false, false, false, false}},
  };

  return routes;
}

bool Contains(std::string_view haystack, std::string_view needle) {
  return haystack.find(needle) != std::string_view::npos;
}

std::optional<std::string> ExtractStringField(std::string_view input,
                                              const std::string& field_name) {
  const std::regex pattern("\"" + field_name + "\"\\s*:\\s*\"([^\"]+)\"");
  std::match_results<std::string_view::const_iterator> match;
  if (!std::regex_search(input.begin(), input.end(), match, pattern) || match.size() < 2) {
    return std::nullopt;
  }

  return std::string(match[1].first, match[1].second);
}

std::optional<std::string> ExtractNumericField(std::string_view input,
                                               const std::string& field_name) {
  const std::regex pattern("\"" + field_name + "\"\\s*:\\s*(\\d+)");
  std::match_results<std::string_view::const_iterator> match;
  if (!std::regex_search(input.begin(), input.end(), match, pattern) || match.size() < 2) {
    return std::nullopt;
  }

  return std::string(match[1].first, match[1].second);
}

std::string ReplaceNumericField(std::string input,
                                const std::string& field_name,
                                std::string_view replacement) {
  const std::regex pattern("(\"" + field_name + "\"\\s*:\\s*)\\d+");
  std::smatch match;
  if (!std::regex_search(input, match, pattern) || match.size() < 2) {
    return input;
  }

  return match.prefix().str() + match[1].str() + std::string(replacement) + match.suffix().str();
}

std::string ReplaceQuotedField(std::string input,
                               const std::string& field_name,
                               std::string_view replacement) {
  const std::regex pattern("(\"" + field_name + "\"\\s*:\\s*\")([^\"]*)(\")");
  std::smatch match;
  if (!std::regex_search(input, match, pattern) || match.size() < 4) {
    return input;
  }

  return match.prefix().str() + match[1].str() + std::string(replacement) + match[3].str() +
         match.suffix().str();
}

}  // namespace

std::optional<ResponsePlan> BuildResponsePlan(std::string_view path,
                                              std::string_view request_body) {
  for (const auto& [needle, plan] : TwoKRoutes()) {
    if (Contains(path, needle)) {
      if (needle == "entitlements/1") {
        break;
      }
      return plan;
    }
  }

  if (Contains(path, "entitlements/1")) {
    const auto action = ExtractStringField(request_body, "action");
    if (!action.has_value()) {
      return std::nullopt;
    }

    if (*action == "steamApi.user.checkAppOwnership") {
      return ResponsePlan{ResponseKind::kTwoK,
                          "checkAppOwnership.json",
                          true,
                          true,
                          true,
                          true};
    }

    if (*action == "entitlementDefs.getFirstPartyMapping") {
      return ResponsePlan{
          ResponseKind::kTwoK, "getFirstPartyMapping.json", true, true, true, false};
    }

    if (*action == "grants.find") {
      return ResponsePlan{ResponseKind::kTwoK, "grantsfind.json", true, true, true, false};
    }

    return std::nullopt;
  }

  for (const auto& [needle, plan] : MultiplayerRoutes()) {
    if (Contains(path, needle)) {
      return plan;
    }
  }

  for (const auto& [needle, plan] : HotfixRoutes()) {
    if (Contains(path, needle)) {
      return plan;
    }
  }

  return std::nullopt;
}

std::string ApplyResponseTransforms(const ResponsePlan& plan,
                                    std::string_view template_body,
                                    std::string_view request_body,
                                    std::string_view steam_id,
                                    std::int64_t unix_time) {
  std::string result(template_body);
  const std::string unix_time_text = std::to_string(unix_time);

  if (plan.replace_app_context) {
    const auto app_context = ExtractNumericField(request_body, "appContext");
    if (app_context.has_value()) {
      result = ReplaceNumericField(std::move(result), "appContext", *app_context);
    }
  }

  if (plan.replace_server_time) {
    result = ReplaceNumericField(std::move(result), "serverTime", unix_time_text);
  }

  if (plan.replace_created_on) {
    result = ReplaceNumericField(std::move(result), "createdOn", unix_time_text);
  }

  if (plan.replace_owner_steam_id) {
    result = ReplaceQuotedField(std::move(result), "ownersteamid", steam_id);
  }

  return result;
}

}  // namespace evolve::server
