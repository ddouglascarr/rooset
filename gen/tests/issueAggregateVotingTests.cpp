
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

namespace rooset_issue_aggregate_voting_tests_tests {


TEST(issue_aggregate_voting_tests, must_be_in_voting_phase_to_cast_ballot)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "SET_ISSUE_BALLOT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [
        "464b1ebb-32c1-460c-8e9e-cccccccccccc"
      ],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ]
    }
  }
})json");
  try {
  JsonUtils::validate(*SetIssueBallotCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueBallotCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, voter_must_be_privileged)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
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
  "type": "SET_ISSUE_BALLOT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [
        "464b1ebb-32c1-460c-8e9e-cccccccccccc"
      ],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ]
    }
  }
})json");
  try {
  JsonUtils::validate(*SetIssueBallotCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueBallotCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, to_unset_a_ballot_the_ballot_must_exist)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
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
  "type": "UNSET_ISSUE_BALLOT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueBallotCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueBallotCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, unset_ballot)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [
        "464b1ebb-32c1-460c-8e9e-cccccccccccc"
      ],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ]
    }
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*IssueBallotUnsetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueBallotUnsetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "UNSET_ISSUE_BALLOT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueBallotCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueBallotCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, unset_ballot_should_fail_on_duplicate)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [
        "464b1ebb-32c1-460c-8e9e-cccccccccccc"
      ],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
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
  "type": "UNSET_ISSUE_BALLOT_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueBallotCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueBallotCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, should_pick_single_winner_in_schulze_ex_1)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1484817333
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1484817320
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 8
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-222222222222",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 4
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-333333333333",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 4
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-444444444444",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-555555555555",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-666666666666",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-222222222222",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-333333333333",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-444444444444",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-555555555555",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-666666666666",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_VOTING_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "winnerId": "464b1ebb-32c1-460c-8e9e-dddddddddddd"
  }
})json");
  try {
  JsonUtils::validate(*IssueVotingPhaseCompletedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueVotingPhaseCompletedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "COMPLETE_ISSUE_VOTING_PHASE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(*CompleteIssueVotingPhaseCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CompleteIssueVotingPhaseCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_voting_tests, should_time_based_tie_break_on_2_schulze_winners_with_delegations)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
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
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "NEW_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
    "unitId": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
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
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-cccccccccccc",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1484817333
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "COMPETING_INITIATIVE_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "initiativeId": "464b1ebb-32c1-460c-8e9e-dddddddddddd",
    "name": "Test Competing Initiative",
    "externalReference": "foobar",
    "content": "mock competing content",
    "textSearchData": "bing, bong",
    "created": 1484817332
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "passingInitiatives": [
      "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb",
      "464b1ebb-32c1-460c-8e9e-cccccccccccc",
      "464b1ebb-32c1-460c-8e9e-dddddddddddd"
    ]
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 2
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-222222222222",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-333333333333",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-444444444444",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-555555555555",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-666666666666",
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
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-777777777777",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "ballot": {
      "approve": [],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-111111111111",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-222222222222",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-333333333333",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ]
      ],
      "abstain": [],
      "disapprove": [
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ]
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_BALLOT_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-444444444444",
    "ballot": {
      "approve": [
        [
          "464b1ebb-32c1-460c-8e9e-dddddddddddd"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
        ],
        [
          "464b1ebb-32c1-460c-8e9e-cccccccccccc"
        ]
      ],
      "abstain": [],
      "disapprove": []
    }
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "trusterId": "464b1ebb-32c1-460c-8e9f-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9f-444444444444"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "trusterId": "464b1ebb-32c1-460c-8e9f-666666666666",
    "trusteeId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "trusterId": "464b1ebb-32c1-460c-8e9f-333333333333",
    "trusteeId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9f-777777777777",
    "trusteeId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_VOTING_PHASE_COMPLETED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333",
    "winnerId": "464b1ebb-32c1-460c-8e9e-bbbbbbbbbbbb"
  }
})json");
  try {
  JsonUtils::validate(*IssueVotingPhaseCompletedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueVotingPhaseCompletedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "COMPLETE_ISSUE_VOTING_PHASE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(*CompleteIssueVotingPhaseCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CompleteIssueVotingPhaseCommand cmd(*cmd_doc);
  
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

}