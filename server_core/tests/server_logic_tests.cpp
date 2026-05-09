#include "evolve/server_logic.hpp"

#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>

void RunResourceStoreTests();

namespace {

using evolve::server::BuildResponsePlan;
using evolve::server::ResponseKind;
using evolve::server::ResponsePlan;
using evolve::server::ApplyResponseTransforms;

void Expect(bool condition, std::string_view message) {
  if (!condition) {
    throw std::runtime_error(std::string(message));
  }
}

void ExpectEqual(std::string_view actual,
                 std::string_view expected,
                 std::string_view message) {
  if (actual != expected) {
    throw std::runtime_error(std::string(message) + "\nactual: " +
                             std::string(actual) + "\nexpected: " +
                             std::string(expected));
  }
}

void ExpectEqual(bool actual, bool expected, std::string_view message) {
  if (actual != expected) {
    throw std::runtime_error(std::string(message));
  }
}

void ExpectEqual(ResponseKind actual,
                 ResponseKind expected,
                 std::string_view message) {
  if (actual != expected) {
    throw std::runtime_error(std::string(message));
  }
}

void TestBuildsDoormanPlan() {
  auto plan = BuildResponsePlan("/doorman/1", R"({"header":{"appContext":1002}})");
  Expect(plan.has_value(), "doorman request should produce a response plan");
  ExpectEqual(plan->kind, ResponseKind::kTwoK, "doorman should map to twok");
  ExpectEqual(plan->asset_name, "doorman.json", "doorman should use doorman.json");
  ExpectEqual(plan->replace_app_context, true, "doorman should replace appContext");
  ExpectEqual(plan->replace_server_time, false, "doorman should not replace serverTime");
}

void TestBuildsEntitlementsPlan() {
  auto plan = BuildResponsePlan(
      "/entitlements/1",
      R"({"header":{"appContext":1002,"action":"steamApi.user.checkAppOwnership"}})");
  Expect(plan.has_value(), "entitlements ownership request should map");
  ExpectEqual(plan->asset_name,
              "checkAppOwnership.json",
              "ownership request should use entitlement asset");
  ExpectEqual(plan->replace_owner_steam_id,
              true,
              "ownership request should replace owner steam id");
  ExpectEqual(plan->replace_created_on,
              true,
              "ownership request should replace createdOn");
}

void TestBuildsHotfixPlan() {
  auto plan = BuildResponsePlan("/Production/252019.252019/micro_patch_version_signed.json", "");
  Expect(plan.has_value(), "hotfix request should produce a response plan");
  ExpectEqual(plan->kind, ResponseKind::kHotfix, "hotfix should map to hotfix kind");
  ExpectEqual(plan->asset_name,
              "micro_patch_version_signed.json",
              "hotfix should preserve expected asset name");
}

void TestAppliesResponseTransforms() {
  ResponsePlan plan;
  plan.kind = ResponseKind::kTwoK;
  plan.replace_app_context = true;
  plan.replace_server_time = true;
  plan.replace_created_on = true;

  const std::string template_body =
      R"({"header":{"appContext":0},"result":{"serverTime":0,"createdOn":0}})";
  const std::string request_body = R"({"header":{"appContext":3456}})";

  const std::string actual =
      ApplyResponseTransforms(plan, template_body, request_body, "76561198000000000", 123456789);

  Expect(actual.find("\"appContext\":3456") != std::string::npos,
         "transforms should update appContext");
  Expect(actual.find("\"serverTime\":123456789") != std::string::npos,
         "transforms should update serverTime");
  Expect(actual.find("\"createdOn\":123456789") != std::string::npos,
         "transforms should update createdOn");
}

void TestAppliesSteamIdOverride() {
  ResponsePlan plan;
  plan.kind = ResponseKind::kTwoK;
  plan.replace_owner_steam_id = true;

  const std::string template_body = R"({"ownersteamid":"0"})";
  const std::string actual =
      ApplyResponseTransforms(plan, template_body, "{}", "76561198000000000", 1);

  Expect(actual.find("76561198000000000") != std::string::npos,
         "steam id transform should replace ownersteamid");
}

}  // namespace

int main() {
  try {
    TestBuildsDoormanPlan();
    TestBuildsEntitlementsPlan();
    TestBuildsHotfixPlan();
    TestAppliesResponseTransforms();
    TestAppliesSteamIdOverride();
    RunResourceStoreTests();
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return 1;
  }

  return 0;
}
