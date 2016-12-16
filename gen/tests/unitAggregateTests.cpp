
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

namespace rooset_unit_aggregate_tests_tests {


TEST(unit_aggregate_tests, unit_creator_can_grant_privileges)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6beb\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"memberId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  PrivilegeGrantedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"GRANT_PRIVILEGE_COMMAND\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6beb\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"memberId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*GrantPrivilegeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GrantPrivilegeCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_tests, unprivileged_member_cannot_create_unit)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"GRANT_PRIVILEGE_COMMAND\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce000\",\"memberId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}");
  try {
  JsonUtils::validate(*GrantPrivilegeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GrantPrivilegeCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_tests, member_without_member_privileges_cannot_grant_privileges)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"memberId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"GRANT_PRIVILEGE_COMMAND\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"memberId\":\"6588b978-d3a8-4ed2-bd65-7157ab1f6016\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}");
  try {
  JsonUtils::validate(*GrantPrivilegeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GrantPrivilegeCommand cmd(*cmd_doc);
  
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


TEST(unit_aggregate_tests, member_with_member_privileges_can_grant_privileges)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"memberId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"memberId\":\"6588b978-d3a8-4ed2-bd65-7157ab1f6016\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}");
  try {
  JsonUtils::validate(*PrivilegeGrantedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  PrivilegeGrantedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"GRANT_PRIVILEGE_COMMAND\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"f00b6b62-72d1-4c5e-974d-8a678f2c252c\",\"memberId\":\"6588b978-d3a8-4ed2-bd65-7157ab1f6016\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}");
  try {
  JsonUtils::validate(*GrantPrivilegeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  GrantPrivilegeCommand cmd(*cmd_doc);
  
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