
#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "framework/AggregateRepositoryMockImpl.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "framework/CommandHandlerTestImpl.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using namespace rooset;

namespace rooset_issue_aggregate_lifecycle_tests_tests {


TEST(issue_aggregate_lifecycle_tests, requester_must_have_voting_rights)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "UNPRIVILEGED_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GIVE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*GiveInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GiveInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, initiative_must_exist_to_give_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ITEM_NOT_FOUND_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GIVE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*GiveInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GiveInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, issue_must_be_in_right_phase_to_give_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-777777777777"
    ]
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ISSUE_STATE_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GIVE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*GiveInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GiveInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, give_initiative_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*InitiativeSupportGivenEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  InitiativeSupportGivenEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GIVE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*GiveInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GiveInitiativeSupportCommand cmd(*cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
}


TEST(issue_aggregate_lifecycle_tests, give_initiative_support_should_fail_on_duplicate)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "CONFLICT_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GIVE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*GiveInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GiveInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, initiative_must_exist_to_revoke_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ITEM_NOT_FOUND_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REVOKE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*RevokeInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RevokeInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, issue_must_be_in_right_phase_to_revoke_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-777777777777"
    ]
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ISSUE_STATE_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REVOKE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*RevokeInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RevokeInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, support_must_have_been_given_to_revoke_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ITEM_NOT_FOUND_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REVOKE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*RevokeInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RevokeInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}


TEST(issue_aggregate_lifecycle_tests, revoke_initiative_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_REVOKED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*InitiativeSupportRevokedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  InitiativeSupportRevokedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REVOKE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*RevokeInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RevokeInitiativeSupportCommand cmd(*cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
  if (result == nullptr) throw invalid_argument("command handler returned nullptr");
  auto resultDoc = result->serialize();
  
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
}


TEST(issue_aggregate_lifecycle_tests, should_fail_on_duplicate_revokation_of_support)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
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
    "memberId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": false,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Policy",
    "description": "The Test Policy",
    "polling": false,
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 5,
    "defeatStrength": "SIMPLE",
    "directMajorityNum": 1,
    "directMajorityDen": 2,
    "directMajorityStrict": true,
    "directMajorityPositive": 1,
    "directMajorityNonNegative": 1,
    "noReverseBeatPath": false,
    "noMultistageMajority": false
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "unitId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-888888888888",
    "name": "Test Initiative",
    "polling": false,
    "externalReference": "",
    "content": "mock content",
    "textSearchData": "foo, bar",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-999999999999",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1483586759
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_GIVEN_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "INITIATIVE_SUPPORT_REVOKED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "COMMAND_EVALUATION_EXCEPTION",
  "error": true,
  "payload": {
    "code": "ITEM_NOT_FOUND_EXCEPTION",
    "message": ""
  }
})json");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REVOKE_INITIATIVE_SUPPORT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*RevokeInitiativeSupportCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RevokeInitiativeSupportCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw invalid_argument("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
  // ignore the message from the test
  (*expectedDoc)["payload"].RemoveMember("message");
  (*resultDoc)["payload"].RemoveMember("message");
      
  bool isPass = *resultDoc == *expectedDoc;
  if (isPass) {
    EXPECT_EQ(*resultDoc, *expectedDoc);
  }  else {
    EXPECT_EQ(*JsonUtils::serialize(*resultDoc),
        *JsonUtils::serialize(*expectedDoc));
  };
  }
}

}