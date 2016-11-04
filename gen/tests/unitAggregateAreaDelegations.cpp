
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

namespace rooset_unit_aggregate_area_delegations_tests {


TEST(unit_aggregate_area_delegations, area_must_exist)
{
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  PrivilegeGrantedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, truster_must_have_voting_right)
{
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
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
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, trustee_must_have_voting_right)
{
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
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
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, set_delegation)
{
  
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  PrivilegeGrantedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  
  auto evt2_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *evt2_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt2 schema invalid");
  }
  AreaCreatedEvent evt2(*evt2_doc);
  mockAggregate.handleEvent(evt2);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"AREA_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*AreaDelegationSetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaDelegationSetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*SetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, unset_delegation_delegation_must_exist)
{
  
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  PrivilegeGrantedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  
  auto evt2_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *evt2_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt2 schema invalid");
  }
  AreaCreatedEvent evt2(*evt2_doc);
  mockAggregate.handleEvent(evt2);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, unset_delegation)
{
  
  
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  PrivilegeGrantedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  
  auto evt2_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *evt2_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt2 schema invalid");
  }
  AreaCreatedEvent evt2(*evt2_doc);
  mockAggregate.handleEvent(evt2);
  
  
  auto evt3_doc = JsonUtils::parse("{\"type\":\"AREA_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*AreaDelegationSetEvent::schema, *evt3_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt3 schema invalid");
  }
  AreaDelegationSetEvent evt3(*evt3_doc);
  mockAggregate.handleEvent(evt3);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"AREA_DELEGATION_UNSET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*AreaDelegationUnsetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AreaDelegationUnsetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_area_delegations, unset_delegation_should_remove_delegation)
{
  
  
  
  
  
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *firstEvent_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("firstEvent schema invalid");
  }
  UnitCreatedEvent firstEvent(*firstEvent_doc);
  UnitAggregate mockAggregate(firstEvent);
  
  auto evt0_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt0_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt0 schema invalid");
  }
  PrivilegeGrantedEvent evt0(*evt0_doc);
  mockAggregate.handleEvent(evt0);
  
  
  auto evt1_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *evt1_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt1 schema invalid");
  }
  PrivilegeGrantedEvent evt1(*evt1_doc);
  mockAggregate.handleEvent(evt1);
  
  
  auto evt2_doc = JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}");
  try {
  JsonUtils::validate(*AreaCreatedEvent::schema, *evt2_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt2 schema invalid");
  }
  AreaCreatedEvent evt2(*evt2_doc);
  mockAggregate.handleEvent(evt2);
  
  
  auto evt3_doc = JsonUtils::parse("{\"type\":\"AREA_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*AreaDelegationSetEvent::schema, *evt3_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt3 schema invalid");
  }
  AreaDelegationSetEvent evt3(*evt3_doc);
  mockAggregate.handleEvent(evt3);
  
  
  auto evt4_doc = JsonUtils::parse("{\"type\":\"AREA_DELEGATION_UNSET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*AreaDelegationUnsetEvent::schema, *evt4_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("evt4 schema invalid");
  }
  AreaDelegationUnsetEvent evt4(*evt4_doc);
  mockAggregate.handleEvent(evt4);
  
  auto repo = make_unique<AggregateRepositoryMockImpl<
      UnitAggregate>>(mockAggregate);
  UnitCommandHandler commandHandler(move(repo));
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_AREA_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*UnsetAreaDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetAreaDelegationCommand cmd(*cmd_doc);
  
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