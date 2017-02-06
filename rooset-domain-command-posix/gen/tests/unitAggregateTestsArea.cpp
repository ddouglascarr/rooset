
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

namespace rooset_unit_aggregate_tests_area_tests {


TEST(unit_aggregate_tests_area, unprivileged_member_cannot_create_area)
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
  "type": "CREATE_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  try {
  JsonUtils::validate(CreateAreaCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(cmd_doc);
  
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


TEST(unit_aggregate_tests_area, member_without_management_rights_cannot_create_area)
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
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": false,
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
  "type": "CREATE_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  try {
  JsonUtils::validate(CreateAreaCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(cmd_doc);
  
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


TEST(unit_aggregate_tests_area, create_area)
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
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json");
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  eventRepository->setMockEvents(givenEvents);
  CommandHandler commandHandler(eventRepository); 
  
  auto expected_doc = JsonUtils::parse(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  try {
  JsonUtils::validate(AreaCreatedEvent::schema, expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaCreatedEvent expected(expected_doc);
  
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "CREATE_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  try {
  JsonUtils::validate(CreateAreaCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(cmd_doc);
  
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


TEST(unit_aggregate_tests_area, area_must_not_already_exist)
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
  "type": "PRIVILEGE_GRANTED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json");
  givenEvents.push_back(u8R"json({
  "type": "AREA_CREATED_EVENT",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
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
  "type": "CREATE_AREA_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "areaId": "464b1ebb-32c1-460c-8e9e-777777777777",
    "requesterId": "464b1ebb-32c1-460c-8e9e-333333333333",
    "name": "test area",
    "description": "the test area",
    "externalReference": "area.com"
  }
})json");
  try {
  JsonUtils::validate(CreateAreaCommand::schema, cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(cmd_doc);
  
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