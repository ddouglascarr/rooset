
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

namespace rooset_concern_tests_tests {


TEST(concern_tests, a_non_manager_user_cannot_create_a_concern)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
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
  "type": "CREATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  try {
  JsonUtils::validate(CreateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_manager_can_create_a_concern_id)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  try {
  JsonUtils::validate(ConcernCreatedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  ConcernCreatedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "CREATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  try {
  JsonUtils::validate(CreateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_non_manager_cannot_deactivate_a_concern)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
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
  "type": "DEACTIVATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(DeactivateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  DeactivateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_concern_must_exist_for_it_to_be_deactivated)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
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
    "code": "ITEM_NOT_FOUND_EXCEPTION",
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
  "type": "DEACTIVATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(DeactivateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  DeactivateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_manager_may_deactivate_a_concern_id)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "CONCERN_DEACTIVATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(ConcernDeactivatedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  ConcernDeactivatedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "DEACTIVATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(DeactivateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  DeactivateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_concern_must_be_active_to_be_deactivated)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_DEACTIVATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
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
    "code": "CONFLICT_EXCEPTION",
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
  "type": "DEACTIVATE_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
  }
})json");
  try {
  JsonUtils::validate(DeactivateConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  DeactivateConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_non_manager_cannot_add_a_concern_to_an_area)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
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
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9f-000000000000"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_concern_must_exist_to_be_added_to_an_area)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
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
    "code": "ITEM_NOT_FOUND_EXCEPTION",
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
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9f-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_concern_must_be_active_to_be_added_to_an_area)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_DEACTIVATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333"
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
    "code": "CONFLICT_EXCEPTION",
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
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_manager_can_add_an_active_concern_to_an_area)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AreaConcernAddedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaConcernAddedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, duplicate_concern_add_should_fail)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
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
    "code": "CONFLICT_EXCEPTION",
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
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_manager_can_remove_a_concern_from_an_area)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_CONCERN_REMOVED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AreaConcernRemovedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaConcernRemovedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "REMOVE_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(RemoveAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RemoveAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, duplicate_removes_should_fail)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_REMOVED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
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
    "code": "ITEM_NOT_FOUND_EXCEPTION",
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
  "type": "REMOVE_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(RemoveAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  RemoveAreaConcernCommand cmd(cmd_doc);
  
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


TEST(concern_tests, a_concern_may_be_re_added_to_an_area_if_it_has_been_removed)
{
  
  vector<string> givenEvents;
  
  givenEvents.push_back(u8R"json({
  "type": "UNIT_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test Unit",
    "description": "The Test Unit",
    "urlParameterName": "test-unit"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "memberId": "464b1ebb-32c1-460c-8e9f-000000000000",
    "votingRight": true,
    "managementRight": false,
    "pollingRight": false,
    "initiativeRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "CONCERN_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "name": "Test concern",
    "description": "The Test Concern",
    "initiativeContentType": "STATEMENT",
    "config": {}
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CONCERN_REMOVED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_CONCERN_ADDED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AreaConcernAddedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaConcernAddedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "ADD_AREA_CONCERN_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-000000000000",
    "areaId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "concernId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "requesterId": "464b1ebb-32c1-460c-8e9e-111111111111"
  }
})json");
  try {
  JsonUtils::validate(AddAreaConcernCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  AddAreaConcernCommand cmd(cmd_doc);
  
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