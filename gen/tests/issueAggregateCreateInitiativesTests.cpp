
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

namespace rooset_issue_aggregate_create_initiatives_tests_tests {


TEST(issue_aggregate_create_initiatives_tests, to_create_new_initiative_member_must_have_initiative_rights)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":false,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_NEW_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}");
  try {
  JsonUtils::validate(*CreateNewInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateNewInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, to_create_competing_initiative_member_must_have_initiative_rights)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":false,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"UNPRIVILEGED_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_COMPETING_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-555555555555\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\",\"name\":\"Test Competing Initiative\",\"externalReference\":\"foobar\",\"content\":\"mock competing content\",\"textSearchData\":\"bing, bong\"}}");
  try {
  JsonUtils::validate(*CreateCompetingInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateCompetingInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, area_must_exsist)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"ITEM_NOT_FOUND_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_NEW_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}");
  try {
  JsonUtils::validate(*CreateNewInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateNewInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, create_new_initiative)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}");
  try {
  JsonUtils::validate(*NewInitiativeCreatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  NewInitiativeCreatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_NEW_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}");
  try {
  JsonUtils::validate(*CreateNewInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateNewInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, should_prevent_double_creation_of_issues)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"CONFLICT_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_NEW_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}");
  try {
  JsonUtils::validate(*CreateNewInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateNewInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, create_competing_initiative)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMPETING_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\",\"name\":\"Test Competing Initiative\",\"externalReference\":\"foobar\",\"content\":\"mock competing content\",\"textSearchData\":\"bing, bong\"}}");
  try {
  JsonUtils::validate(*CompetingInitiativeCreatedEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CompetingInitiativeCreatedEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_COMPETING_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\",\"name\":\"Test Competing Initiative\",\"externalReference\":\"foobar\",\"content\":\"mock competing content\",\"textSearchData\":\"bing, bong\"}}");
  try {
  JsonUtils::validate(*CreateCompetingInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateCompetingInitiativeCommand cmd(*cmd_doc);
  
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


TEST(issue_aggregate_create_initiatives_tests, should_prevent_double_creation_of_initiatives)
{
  
  vector<unique_ptr<Document>> givenEvents;
  
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"AREA_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"test area\",\"description\":\"the test area\",\"externalReference\":\"area.com\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":true}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"PRIVILEGE_GRANTED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"memberId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"pollingRight\":true,\"votingRight\":true,\"initiativeRight\":true,\"managementRight\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"NEW_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-444444444444\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"unitId\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"areaId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-888888888888\",\"name\":\"Test Initiative\",\"polling\":false,\"externalReference\":\"\",\"content\":\"mock content\",\"textSearchData\":\"foo, bar\"}}"));
  givenEvents.push_back(JsonUtils::parse("{\"type\":\"COMPETING_INITIATIVE_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\",\"name\":\"Test Competing Initiative\",\"externalReference\":\"foobar\",\"content\":\"mock competing content\",\"textSearchData\":\"bing, bong\"}}"));
  CommandHandlerTestImpl commandHandler(givenEvents); 
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"COMMAND_EVALUATION_EXCEPTION\",\"error\":true,\"payload\":{\"code\":\"CONFLICT_EXCEPTION\",\"message\":\"\"}}");
  try {
  JsonUtils::validate(*CommandEvaluationException::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  CommandEvaluationException expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"CREATE_COMPETING_INITIATIVE_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-666666666666\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-999999999999\",\"initiativeId\":\"464b1ebb-32c1-460c-8e9e-000000000000\",\"name\":\"Test Competing Initiative\",\"externalReference\":\"foobar\",\"content\":\"mock competing content\",\"textSearchData\":\"bing, bong\"}}");
  try {
  JsonUtils::validate(*CreateCompetingInitiativeCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  CreateCompetingInitiativeCommand cmd(*cmd_doc);
  
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