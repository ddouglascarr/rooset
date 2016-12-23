
#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "framework/AggregateRepositoryMockImpl.h"
#include "framework/AggregateRepositoryEmptyMockImpl.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"
#include "framework/CommandHandlerTestImpl.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using namespace rooset;

namespace rooset_unit_aggregate_area_delegations_tests {


TEST(unit_aggregate_area_delegations, area_must_exist)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, truster_must_have_voting_right)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, trustee_must_have_voting_right)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, set_delegation)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, unset_delegation_delegation_must_exist)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, unset_delegation)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, unset_delegation_should_remove_delegation)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_DELEGATION_UNSET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
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


TEST(unit_aggregate_area_delegations, set_issue_delegation_should_require_voting_privilege_of_trustee)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\"}}");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
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


TEST(unit_aggregate_area_delegations, set_issue_delegation_should_require_voting_privilege_of_truster)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*SetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetIssueDelegationCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
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


TEST(unit_aggregate_area_delegations, set_issue_delegation)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"ISSUE_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
  try {
  JsonUtils::validate(*IssueDelegationSetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueDelegationSetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}");
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


TEST(unit_aggregate_area_delegations, unset_issue_delegation_should_fail_if_no_delegation_set)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
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


TEST(unit_aggregate_area_delegations, unset_issue_delegation)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ISSUE_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"ISSUE_DELEGATION_UNSET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*IssueDelegationUnsetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  IssueDelegationUnsetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
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


TEST(unit_aggregate_area_delegations, unset_issue_delegation_should_fail_on_duplicate)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ISSUE_DELEGATION_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"trusteeId\":\"464b1ebb-32c1-460c-8e9e-444444444444\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ISSUE_DELEGATION_UNSET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"trusterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UNSET_ISSUE_DELEGATION_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\"}}");
  try {
  JsonUtils::validate(*UnsetIssueDelegationCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UnsetIssueDelegationCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
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