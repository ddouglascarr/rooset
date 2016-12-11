
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

namespace rooset_unit_aggregate_policy_tests {


TEST(unit_aggregate_policy, set_policy)
{
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
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
  
  auto expected_doc = JsonUtils::parse("{\"type\":\"UNIT_POLICY_SET_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}");
  try {
  JsonUtils::validate(*UnitPolicySetEvent::schema, *expected_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("expected schema invalid");
  }
  UnitPolicySetEvent expected(*expected_doc);
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_UNIT_POLICY_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}");
  try {
  JsonUtils::validate(*SetUnitPolicyCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetUnitPolicyCommand cmd(*cmd_doc);
  
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



TEST(unit_aggregate_policy, requester_must_have_managment_rights)
{
  
  
  
  auto firstEvent_doc = JsonUtils::parse("{\"type\":\"UNIT_CREATED_EVENT\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-333333333333\",\"name\":\"Test Unit\",\"description\":\"The Test Unit\"}}");
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
  
  auto cmd_doc = JsonUtils::parse("{\"type\":\"SET_UNIT_POLICY_COMMAND\",\"payload\":{\"id\":\"464b1ebb-32c1-460c-8e9e-111111111111\",\"requesterId\":\"464b1ebb-32c1-460c-8e9e-777777777777\",\"policyId\":\"464b1ebb-32c1-460c-8e9e-222222222222\",\"name\":\"Test Policy\",\"description\":\"The Test Policy\",\"polling\":false,\"maxAdmissionTime\":604800000,\"minAdmissionTime\":0,\"discussionTime\":604800000,\"verificationTime\":604800000,\"votingTime\":604800000,\"issueQuorumNum\":1,\"issueQuorumDen\":10,\"defeatStrength\":\"SIMPLE\",\"directMajorityNum\":1,\"directMajorityDen\":2,\"directMajorityStrict\":true,\"directMajorityPositive\":1,\"directMajorityNonNegative\":1,\"noReverseBeatPath\":false,\"noMultistageMajority\":false}}");
  try {
  JsonUtils::validate(*SetUnitPolicyCommand::schema, *cmd_doc);
  } catch (invalid_argument e) {
    throw invalid_argument("cmd schema invalid");
  }
  SetUnitPolicyCommand cmd(*cmd_doc);
  
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