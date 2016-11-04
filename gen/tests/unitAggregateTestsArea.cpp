
#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "framework/AggregateRepositoryMockImpl.h"
#include "framework/AggregateRepositoryEmptyMockImpl.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "exceptions/CommandEvaluationException.h"
#include "aggregates/UnitAggregate.h"
#include "aggregates/UnitCommandHandler.h"

using namespace std;
using namespace rooset;

namespace rooset_unit_aggregate_tests_area_tests {


TEST(unit_aggregate_tests_area, unprivileged_member_cannot_create_area)
{
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_AREA_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*CreateAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
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



TEST(unit_aggregate_tests_area, member_without_management_rights_cannot_create_area)
{
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_AREA_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*CreateAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
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



TEST(unit_aggregate_tests_area, create_area)
{
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaCreatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_AREA_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*CreateAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(*cmd_doc);
  
  auto result = commandHandler.evaluate(cmd);
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



TEST(unit_aggregate_tests_area, area_must_not_already_exist)
{
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  AreaCreatedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"CONFLICT_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_AREA_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*CreateAreaCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAreaCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
  // if docs don't match, assess the json output to make useful error report
  auto expectedDoc = expected.serialize();
  
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