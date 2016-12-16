
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

namespace rooset_member_aggregate_tests_tests {


TEST(member_aggregate_tests, admin_member_created_event)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"ADMIN_MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"Adam Admin\",\"notifyEmail\":\"adam@admin.com\"}}");
  try {
  JsonUtils::validate(*AdminMemberCreatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  AdminMemberCreatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_ADMIN_MEMBER_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"Adam Admin\",\"notifyEmail\":\"adam@admin.com\"}}");
  try {
  JsonUtils::validate(*CreateAdminMemberCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateAdminMemberCommand cmd(*cmd_doc);
  
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


TEST(member_aggregate_tests, update_member_password)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ADMIN_MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"Adam Admin\",\"notifyEmail\":\"adam@admin.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"MEMBER_PASSWORD_UPDATED_EVENT\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"newPassword\":\"pw2\"}}");
  try {
  JsonUtils::validate(*MemberPasswordUpdatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  MemberPasswordUpdatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UPDATE_MEMBER_PASSWORD_COMMAND\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"oldPassword\":\"pw1\",\"newPassword\":\"pw2\"}}");
  try {
  JsonUtils::validate(*UpdateMemberPasswordCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UpdateMemberPasswordCommand cmd(*cmd_doc);
  
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


TEST(member_aggregate_tests, update_member_password_should_throw_on_wrong_old_password)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ADMIN_MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"Adam Admin\",\"notifyEmail\":\"adam@admin.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"CONFLICT_EXCEPTION\",\"message\":\"The old password does not match\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"UPDATE_MEMBER_PASSWORD_COMMAND\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"oldPassword\":\"pw-wrong\",\"newPassword\":\"pw2\"}}");
  try {
  JsonUtils::validate(*UpdateMemberPasswordCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  UpdateMemberPasswordCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
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


TEST(member_aggregate_tests, admin_member_can_create_unit)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"ADMIN_MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"Adam Admin\",\"notifyEmail\":\"adam@admin.com\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*UnitCreatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  UnitCreatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_UNIT_COMMAND\",\"payload\":{\"id\":\"0c1fe645-4f57-4cfa-88d5-b2973f3f6bec\",\"requesterId\":\"86998399-3d86-4e0b-a2a5-6490056ce43e\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*CreateUnitCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateUnitCommand cmd(*cmd_doc);
  
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


TEST(member_aggregate_tests, non_admin_member_can_not_create_unit)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"54b692a4-77d2-4ce5-b785-3880e2f7a276\",\"login\":\"normal\",\"password\":\"pw1\",\"name\":\"normal user\",\"notifyEmail\":\"normal@user.com\",\"activated\":1477743073}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"Only admin users can create units\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_UNIT_COMMAND\",\"payload\":{\"id\":\"ccb143ff-0427-4cf5-88a0-1b1daf81ef2b\",\"requesterId\":\"54b692a4-77d2-4ce5-b785-3880e2f7a276\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
  try {
  JsonUtils::validate(*CreateUnitCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateUnitCommand cmd(*cmd_doc);
  
  try {
    commandHandler.evaluate(cmd);
    throw string("An exception was expected");
  } catch(CommandEvaluationException e) {
    auto resultDoc = e.serialize();
    
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

}