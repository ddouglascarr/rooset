
#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "ratk/JsonUtils.h"
#include "ratk/IdToolsImpl.h"
#include "ratk/EventRepositoryMockImpl.h"
#include "aggregates/CommandHandler.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using namespace rooset;
using ::testing::NiceMock;

namespace rooset_support_caculation_tests_tests {


TEST(support_caculation_tests, test_compiles)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  try {
  JsonUtils::validate(AreaDelegationSetEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaDelegationSetEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "SET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  try {
  JsonUtils::validate(SetAreaDelegationCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
}


TEST(support_caculation_tests, issue_must_be_in_admission_phase_for_quorum_assessment)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_PASSED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ISSUE_STATE_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(CommandEvaluationException::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ASSESS_ISSUE_ADMISSION_QUORUM_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(AssessIssueAdmissionQuorumCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AssessIssueAdmissionQuorumCommand cmd(cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test, and log it if the test fails
  const string msg = (*resultDoc)["payload"]["message"].GetString();
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
  if (*expectedDoc != *resultDoc) cout << msg;
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(support_caculation_tests, issue_must_be_in_admission_phase_for_quorum_complete)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_PASSED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ISSUE_STATE_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(CommandEvaluationException::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "COMPLETE_ISSUE_ADMISSION_PHASE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(CompleteIssueAdmissionPhaseCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CompleteIssueAdmissionPhaseCommand cmd(cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test, and log it if the test fails
  const string msg = (*resultDoc)["payload"]["message"].GetString();
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
  if (*expectedDoc != *resultDoc) cout << msg;
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(support_caculation_tests, short_of_quorum_should_continue_on_assessment)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_CONTINUED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(IssueAdmissionQuorumContinuedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueAdmissionQuorumContinuedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ASSESS_ISSUE_ADMISSION_QUORUM_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(AssessIssueAdmissionQuorumCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AssessIssueAdmissionQuorumCommand cmd(cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
}


TEST(support_caculation_tests, short_of_quorum_should_fail_on_complete)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_FAILED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(IssueAdmissionQuorumFailedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueAdmissionQuorumFailedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "COMPLETE_ISSUE_ADMISSION_PHASE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(CompleteIssueAdmissionPhaseCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CompleteIssueAdmissionPhaseCommand cmd(cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
}


TEST(support_caculation_tests, over_quorum_should_pass_on_assessment)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_PASSED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(IssueAdmissionQuorumPassedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueAdmissionQuorumPassedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ASSESS_ISSUE_ADMISSION_QUORUM_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(AssessIssueAdmissionQuorumCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AssessIssueAdmissionQuorumCommand cmd(cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
}


TEST(support_caculation_tests, over_quorum_should_pass_on_complete)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
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
})json");
  givenEvents.push_back(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-ffffffffffff"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-eeeeeeeeeeee"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-aaaaaaaaaaaa",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-ffffffffffff",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_PASSED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(IssueAdmissionQuorumPassedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueAdmissionQuorumPassedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "COMPLETE_ISSUE_ADMISSION_PHASE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
  }
})json");
  try {
  JsonUtils::validate(CompleteIssueAdmissionPhaseCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CompleteIssueAdmissionPhaseCommand cmd(cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
}

}