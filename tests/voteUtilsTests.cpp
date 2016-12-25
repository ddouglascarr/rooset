#include "gtest/gtest.h"

#include <vector>
#include <memory>
#include "rapidjson/document.h"
#include "aggregates/UnitAggregate.h"
#include "aggregates/IssueAggregate.h"
#include "events/EventUtils.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "aggregates/VoteUtils.h"

using namespace std;
using namespace rooset;

namespace roosetVoteUtilsTests {

  TEST(VoteUtils, calcSupportWeight)
  {
  // pasted from supportCalculationTests.cpp
  vector<unique_ptr<rapidjson::Document>> givenEvents;

  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 3
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 4
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json"));

    IdToolsImpl idTools;

    // apply events to aggregates
    unique_ptr<UnitAggregate> unit = nullptr;
    unique_ptr<IssueAggregate> issue = nullptr;
    for (const auto& e : givenEvents) {
      EventUtils::applyEvent<UnitAggregate>(unit, *e, []() {});
      EventUtils::applyEvent<IssueAggregate>(issue, *e, []() {});
    }

    auto supportWeightA = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightC = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-cccccccccccc"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightF = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-ffffffffffff"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    auto supportWeightE = VoteUtils::calcSupportWeight(
      idTools.parse("464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"),
      issue->getInitiatives().at(idTools.parse("464b1ebb-32c1-460c-8e9e-777777777777")),
      *issue,
      unit->getAreas().at(idTools.parse("464b1ebb-32c1-460c-8e9e-333333333333")),
      *unit);

    EXPECT_EQ(supportWeightA, 3);
    EXPECT_EQ(supportWeightC, 3);
    EXPECT_EQ(supportWeightF, 2);
    EXPECT_EQ(supportWeightE, 1);
  }

  TEST(VoteUtils, calcTotalVoteWeight)
  {
    IdToolsImpl idTools;
    map<uuid, rooset::MemberPrivilege> privileges;
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 1 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 2 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 3 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 4 };
    privileges[idTools.generateUniqueId()] = { true, true, true, true, 5 };
    privileges[idTools.generateUniqueId()] = { true, false, true, true, 15 };

    const auto totalVoteWeight = VoteUtils::calcTotalVoteWeight(privileges);
    EXPECT_EQ(totalVoteWeight, 15);
  }
}

TEST(VoteUtils, isAdmissionQuorumPassed)
{
  rooset::Policy policy{
    "test policy",
    true,
    1,
    10
  };

  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 100, 10), true);
  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 101, 10), false);
  EXPECT_EQ(VoteUtils::isAdmissionQuorumPassed(policy, 12, 7), true);
}