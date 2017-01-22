
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

namespace rooset_unit_aggregate_policy_tests {


TEST(unit_aggregate_policy, set_policy)
{
  
  vector<Document> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "Test Unit",
    "description": "The Test Unit"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
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
  try {
  JsonUtils::validate(UnitPolicySetEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  UnitPolicySetEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "SET_UNIT_POLICY_COMMAND",
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
  try {
  JsonUtils::validate(SetUnitPolicyCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetUnitPolicyCommand cmd(cmd_doc);
  
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


TEST(unit_aggregate_policy, requester_must_have_managment_rights)
{
  
  vector<Document> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "Test Unit",
    "description": "The Test Unit"
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
  JsonUtils::validate(CommandEvaluationException::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "SET_UNIT_POLICY_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-777777777777",
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
  try {
  JsonUtils::validate(SetUnitPolicyCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetUnitPolicyCommand cmd(cmd_doc);
  
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
    EXPECT_EQ(JsonUtils::serialize(*resultDoc),
        JsonUtils::serialize(*expectedDoc));
  };
  }
}

}