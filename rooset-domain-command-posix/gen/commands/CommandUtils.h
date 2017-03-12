
#pragma once
#include <functional>
#include <memory>
#include "exceptions/CommandEvaluationException.h"
#include "rapidjson/document.h"
#include "ratk/MessageUtils.h"
#include "ratk/JsonUtils.h"

#include "commands/AddAreaConcernCommand.h"
#include "commands/AddPolicyCommand.h"
#include "commands/AssessIssueAdmissionQuorumCommand.h"
#include "commands/BlockDelegationForAreaCommand.h"
#include "commands/BlockDelegationForIssueCommand.h"
#include "commands/CompleteIssueAdmissionPhaseCommand.h"
#include "commands/CompleteIssueDiscussionPhaseCommand.h"
#include "commands/CompleteIssueVerificationPhaseCommand.h"
#include "commands/CompleteIssueVotingPhaseCommand.h"
#include "commands/CreateAreaCommand.h"
#include "commands/CreateCompetingInitiativeCommand.h"
#include "commands/CreateConcernCommand.h"
#include "commands/CreateNewInitiativeCommand.h"
#include "commands/CreateUnitCommand.h"
#include "commands/DeactivateConcernCommand.h"
#include "commands/DeactivatePolicyCommand.h"
#include "commands/GiveInitiativeSupportCommand.h"
#include "commands/GrantPrivilegeCommand.h"
#include "commands/RemoveAreaConcernCommand.h"
#include "commands/RevokeInitiativeSupportCommand.h"
#include "commands/SetAreaDelegationCommand.h"
#include "commands/SetIssueBallotCommand.h"
#include "commands/SetIssueDelegationCommand.h"
#include "commands/SetUnitDelegationCommand.h"
#include "commands/UnblockDelegationForAreaCommand.h"
#include "commands/UnblockDelegationForIssueCommand.h"
#include "commands/UnsetAreaDelegationCommand.h"
#include "commands/UnsetIssueBallotCommand.h"
#include "commands/UnsetIssueDelegationCommand.h"
#include "commands/UnsetUnitDelegationCommand.h"

namespace rooset {

  class CommandUtils {
  public:
    template<typename CommandHandler>
    inline static unique_ptr<ProjectEvent<Document>> evaluate(
        CommandHandler& commandHandler,
        const rapidjson::Document& d)
    {
      const string msgType = d["type"].GetString();

      
      if (msgType == "ADD_AREA_CONCERN_COMMAND") {
        JsonUtils::validate(AddAreaConcernCommand::schema, d);
        const AddAreaConcernCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "ADD_POLICY_COMMAND") {
        JsonUtils::validate(AddPolicyCommand::schema, d);
        const AddPolicyCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "ASSESS_ISSUE_ADMISSION_QUORUM_COMMAND") {
        JsonUtils::validate(AssessIssueAdmissionQuorumCommand::schema, d);
        const AssessIssueAdmissionQuorumCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "BLOCK_DELEGATION_FOR_AREA_COMMAND") {
        JsonUtils::validate(BlockDelegationForAreaCommand::schema, d);
        const BlockDelegationForAreaCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "BLOCK_DELEGATION_FOR_ISSUE_COMMAND") {
        JsonUtils::validate(BlockDelegationForIssueCommand::schema, d);
        const BlockDelegationForIssueCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "COMPLETE_ISSUE_ADMISSION_PHASE_COMMAND") {
        JsonUtils::validate(CompleteIssueAdmissionPhaseCommand::schema, d);
        const CompleteIssueAdmissionPhaseCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "COMPLETE_ISSUE_DISCUSSION_PHASE_COMMAND") {
        JsonUtils::validate(CompleteIssueDiscussionPhaseCommand::schema, d);
        const CompleteIssueDiscussionPhaseCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "COMPLETE_ISSUE_VERIFICATION_PHASE_COMMAND") {
        JsonUtils::validate(CompleteIssueVerificationPhaseCommand::schema, d);
        const CompleteIssueVerificationPhaseCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "COMPLETE_ISSUE_VOTING_PHASE_COMMAND") {
        JsonUtils::validate(CompleteIssueVotingPhaseCommand::schema, d);
        const CompleteIssueVotingPhaseCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "CREATE_AREA_COMMAND") {
        JsonUtils::validate(CreateAreaCommand::schema, d);
        const CreateAreaCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "CREATE_COMPETING_INITIATIVE_COMMAND") {
        JsonUtils::validate(CreateCompetingInitiativeCommand::schema, d);
        const CreateCompetingInitiativeCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "CREATE_CONCERN_COMMAND") {
        JsonUtils::validate(CreateConcernCommand::schema, d);
        const CreateConcernCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "CREATE_NEW_INITIATIVE_COMMAND") {
        JsonUtils::validate(CreateNewInitiativeCommand::schema, d);
        const CreateNewInitiativeCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "CREATE_UNIT_COMMAND") {
        JsonUtils::validate(CreateUnitCommand::schema, d);
        const CreateUnitCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "DEACTIVATE_CONCERN_COMMAND") {
        JsonUtils::validate(DeactivateConcernCommand::schema, d);
        const DeactivateConcernCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "DEACTIVATE_POLICY_COMMAND") {
        JsonUtils::validate(DeactivatePolicyCommand::schema, d);
        const DeactivatePolicyCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "GIVE_INITIATIVE_SUPPORT_COMMAND") {
        JsonUtils::validate(GiveInitiativeSupportCommand::schema, d);
        const GiveInitiativeSupportCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "GRANT_PRIVILEGE_COMMAND") {
        JsonUtils::validate(GrantPrivilegeCommand::schema, d);
        const GrantPrivilegeCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "REMOVE_AREA_CONCERN_COMMAND") {
        JsonUtils::validate(RemoveAreaConcernCommand::schema, d);
        const RemoveAreaConcernCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "REVOKE_INITIATIVE_SUPPORT_COMMAND") {
        JsonUtils::validate(RevokeInitiativeSupportCommand::schema, d);
        const RevokeInitiativeSupportCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "SET_AREA_DELEGATION_COMMAND") {
        JsonUtils::validate(SetAreaDelegationCommand::schema, d);
        const SetAreaDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "SET_ISSUE_BALLOT_COMMAND") {
        JsonUtils::validate(SetIssueBallotCommand::schema, d);
        const SetIssueBallotCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "SET_ISSUE_DELEGATION_COMMAND") {
        JsonUtils::validate(SetIssueDelegationCommand::schema, d);
        const SetIssueDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "SET_UNIT_DELEGATION_COMMAND") {
        JsonUtils::validate(SetUnitDelegationCommand::schema, d);
        const SetUnitDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNBLOCK_DELEGATION_FOR_AREA_COMMAND") {
        JsonUtils::validate(UnblockDelegationForAreaCommand::schema, d);
        const UnblockDelegationForAreaCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNBLOCK_DELEGATION_FOR_ISSUE_COMMAND") {
        JsonUtils::validate(UnblockDelegationForIssueCommand::schema, d);
        const UnblockDelegationForIssueCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNSET_AREA_DELEGATION_COMMAND") {
        JsonUtils::validate(UnsetAreaDelegationCommand::schema, d);
        const UnsetAreaDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNSET_ISSUE_BALLOT_COMMAND") {
        JsonUtils::validate(UnsetIssueBallotCommand::schema, d);
        const UnsetIssueBallotCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNSET_ISSUE_DELEGATION_COMMAND") {
        JsonUtils::validate(UnsetIssueDelegationCommand::schema, d);
        const UnsetIssueDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      if (msgType == "UNSET_UNIT_DELEGATION_COMMAND") {
        JsonUtils::validate(UnsetUnitDelegationCommand::schema, d);
        const UnsetUnitDelegationCommand cmd(d);
        return commandHandler.evaluate(cmd);
      }

      throw CommandEvaluationException(
          ExceptionCode::GENERAL_PROJECT_EXCEPTION,
          "There is no ifBlock for " + msgType + ". Run make again, and check the generateCommandUtils script");
    }

  };
}


  