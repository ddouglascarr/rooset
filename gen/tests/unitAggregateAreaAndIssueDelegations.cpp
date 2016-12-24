
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

namespace rooset_unit_aggregate_area_and_issue_delegations_tests {


TEST(unit_aggregate_area_and_issue_delegations, area_must_exist)
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
  "type": "SET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, truster_must_have_voting_right)
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
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "SET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, trustee_must_have_voting_right)
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
  "type": "SET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*AreaDelegationSetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaDelegationSetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "SET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_area_delegation_should_remove_block)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
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
  "type": "UNBLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_delegation_delegation_must_exist)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "UNSET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*AreaDelegationUnsetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaDelegationUnsetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "UNSET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_delegation_should_remove_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "AREA_DELEGATION_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
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
  "type": "UNSET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_area_delegation_should_unset_area_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "AREA_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
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
  "type": "UNSET_AREA_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, area_must_exist_to_block_area_delegation)
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
  "type": "BLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_area_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*DelegationBlockedForAreaEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  DelegationBlockedForAreaEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "BLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_area_for_delegation_should_fail_on_duplicate)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
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
  "type": "BLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, area_must_exist_to_unblock_area_delegation)
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
  "type": "UNBLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_must_exist_to_unblock_area_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "UNBLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unblock_area_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
  "type": "DELEGATION_BLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "DELEGATION_UNBLOCKED_FOR_AREA_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "trusterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*DelegationUnblockedForAreaEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  DelegationUnblockedForAreaEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "UNBLOCK_DELEGATION_FOR_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForAreaCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_issue_delegation_should_require_voting_privilege_of_trustee)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
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
  "type": "SET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-000000000000"
  }
})json");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_issue_delegation_should_require_voting_privilege_of_truster)
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
    "textSearchData": "foo, bar"
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
  "type": "SET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_issue_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*IssueDelegationSetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueDelegationSetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "SET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_issue_delegation_should_remove_conflicting_block)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
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
  "type": "UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_issue_delegation_should_fail_if_no_delegation_set)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
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
  "type": "UNSET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_issue_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*IssueDelegationUnsetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueDelegationUnsetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "UNSET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_issue_delegation_should_fail_on_duplicate)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_UNSET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
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
  "type": "UNSET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, set_issue_delegation_should_fail_if_issue_not_open)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_FAILED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
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
  "type": "SET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unset_issue_delegation_should_fail_if_issue_not_open)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_FAILED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
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
  "type": "UNSET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_issue_delegation_should_fail_if_issue_not_open)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_FAILED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
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
  "type": "BLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, member_must_have_voting_rights_to_block_delegation_for_issue)
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
    "textSearchData": "foo, bar"
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
  "type": "BLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_issue_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*DelegationBlockedForIssueEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  DelegationBlockedForIssueEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "BLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_issue_delegation_should_fail_on_duplicate)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
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
  "type": "BLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*BlockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  BlockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, block_issue_delegation_should_remove_conflicting_area_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_DELEGATION_SET_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555",
    "trusteeId": "464b1ebb-32c1-460c-8e9e-444444444444"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
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
  "type": "UNSET_ISSUE_DELEGATION_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unblock_issue_delegation_should_fail_if_issue_not_open)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "ISSUE_ADMISSION_QUORUM_FAILED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666"
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
  "type": "UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unblock_issue_delegation_should_fail_if_issue_no_block_exists)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
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
  "type": "UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForIssueCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_area_and_issue_delegations, unblock_issue_delegation)
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
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
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
    "textSearchData": "foo, bar"
  }
})json"));
  givenEvents.push_back(JsonUtils::parse(u8R"json({
  "type": "DELEGATION_BLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "DELEGATION_UNBLOCKED_FOR_ISSUE_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "trusterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*DelegationUnblockedForIssueEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  DelegationUnblockedForIssueEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-666666666666",
    "requesterId": "464b1ebb-32c1-460c-8e9e-555555555555"
  }
})json");
  try {
  JsonUtils::validate(*UnblockDelegationForIssueCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnblockDelegationForIssueCommand cmd(*cmd_doc);
  
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