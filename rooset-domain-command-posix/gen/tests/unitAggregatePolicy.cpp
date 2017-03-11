
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

namespace rooset_unit_aggregate_policy_tests {


TEST(unit_aggregate_policy, add_policy)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "UNIT_POLICY_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "votingAlgorithm": "SCHULZE",
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "initiativeQuorumNum": 1,
    "initiativeQuorumDen": 10
  }
})json");
  try {
  JsonUtils::validate(UnitPolicyAddedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  UnitPolicyAddedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ADD_POLICY_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "votingAlgorithm": "SCHULZE",
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "initiativeQuorumNum": 1,
    "initiativeQuorumDen": 10
  }
})json");
  try {
  JsonUtils::validate(AddPolicyCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddPolicyCommand cmd(cmd_doc);
  
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
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
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
  "type": "ADD_POLICY_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "policyId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "Test Policy",
    "description": "The Test Policy",
    "votingAlgorithm": "SCHULZE",
    "maxAdmissionTime": 604800000,
    "minAdmissionTime": 0,
    "discussionTime": 604800000,
    "verificationTime": 604800000,
    "votingTime": 604800000,
    "issueQuorumNum": 1,
    "issueQuorumDen": 10,
    "initiativeQuorumNum": 1,
    "initiativeQuorumDen": 10
  }
})json");
  try {
  JsonUtils::validate(AddPolicyCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddPolicyCommand cmd(cmd_doc);
  
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

}