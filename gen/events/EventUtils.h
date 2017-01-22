
#pragma once
#include <functional>
#include "exceptions/CommandEvaluationException.h"
#include "rapidjson/document.h"
#include "framework/MessageUtils.h"
#include "framework/JsonUtils.h"

#include "events/AdminMemberCreatedEvent.h"
#include "events/AreaCreatedEvent.h"
#include "events/MemberCreatedEvent.h"
#include "events/MemberPasswordUpdatedEvent.h"
#include "events/UnitCreatedEvent.h"
#include "events/AreaDelegationSetEvent.h"
#include "events/AreaDelegationUnsetEvent.h"
#include "events/CompetingInitiativeCreatedEvent.h"
#include "events/DelegationBlockedForAreaEvent.h"
#include "events/DelegationBlockedForIssueEvent.h"
#include "events/DelegationUnblockedForAreaEvent.h"
#include "events/DelegationUnblockedForIssueEvent.h"
#include "events/InitiativeSupportGivenEvent.h"
#include "events/InitiativeSupportRevokedEvent.h"
#include "events/IssueAdmissionQuorumContinuedEvent.h"
#include "events/IssueAdmissionQuorumFailedEvent.h"
#include "events/IssueAdmissionQuorumPassedEvent.h"
#include "events/IssueBallotSetEvent.h"
#include "events/IssueBallotUnsetEvent.h"
#include "events/IssueDelegationSetEvent.h"
#include "events/IssueDelegationUnsetEvent.h"
#include "events/IssueDiscussionPhaseCompletedEvent.h"
#include "events/IssueVerificationPhaseCompletedEvent.h"
#include "events/IssueVotingPhaseCompletedEvent.h"
#include "events/NewInitiativeCreatedEvent.h"
#include "events/PrivilegeGrantedEvent.h"
#include "events/UnitDelegationSetEvent.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "events/UnitPolicySetEvent.h"

namespace rooset {

  class EventUtils {
  public:
    template<typename Aggregate>
    static void applyEvent(
        unique_ptr<Aggregate>& aggregate,
        const rapidjson::Document& d,
        std::function<void()> onMethodMissing)
    {
      const string msgType = d["type"].GetString();

      
      if (msgType == "ADMIN_MEMBER_CREATED_EVENT") {
        JsonUtils::validate(AdminMemberCreatedEvent::schema, d);
        const AdminMemberCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AdminMemberCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_CREATED_EVENT") {
        JsonUtils::validate(AreaCreatedEvent::schema, d);
        const AreaCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "MEMBER_CREATED_EVENT") {
        JsonUtils::validate(MemberCreatedEvent::schema, d);
        const MemberCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, MemberCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "MEMBER_PASSWORD_UPDATED_EVENT") {
        JsonUtils::validate(MemberPasswordUpdatedEvent::schema, d);
        const MemberPasswordUpdatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, MemberPasswordUpdatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_CREATED_EVENT") {
        JsonUtils::validate(UnitCreatedEvent::schema, d);
        const UnitCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_DELEGATION_SET_EVENT") {
        JsonUtils::validate(AreaDelegationSetEvent::schema, d);
        const AreaDelegationSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaDelegationSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_DELEGATION_UNSET_EVENT") {
        JsonUtils::validate(AreaDelegationUnsetEvent::schema, d);
        const AreaDelegationUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaDelegationUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "COMPETING_INITIATIVE_CREATED_EVENT") {
        JsonUtils::validate(CompetingInitiativeCreatedEvent::schema, d);
        const CompetingInitiativeCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, CompetingInitiativeCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "DELEGATION_BLOCKED_FOR_AREA_EVENT") {
        JsonUtils::validate(DelegationBlockedForAreaEvent::schema, d);
        const DelegationBlockedForAreaEvent evt(d);
        MessageUtils::applyEvent<Aggregate, DelegationBlockedForAreaEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "DELEGATION_BLOCKED_FOR_ISSUE_EVENT") {
        JsonUtils::validate(DelegationBlockedForIssueEvent::schema, d);
        const DelegationBlockedForIssueEvent evt(d);
        MessageUtils::applyEvent<Aggregate, DelegationBlockedForIssueEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "DELEGATION_UNBLOCKED_FOR_AREA_EVENT") {
        JsonUtils::validate(DelegationUnblockedForAreaEvent::schema, d);
        const DelegationUnblockedForAreaEvent evt(d);
        MessageUtils::applyEvent<Aggregate, DelegationUnblockedForAreaEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "DELEGATION_UNBLOCKED_FOR_ISSUE_EVENT") {
        JsonUtils::validate(DelegationUnblockedForIssueEvent::schema, d);
        const DelegationUnblockedForIssueEvent evt(d);
        MessageUtils::applyEvent<Aggregate, DelegationUnblockedForIssueEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "INITIATIVE_SUPPORT_GIVEN_EVENT") {
        JsonUtils::validate(InitiativeSupportGivenEvent::schema, d);
        const InitiativeSupportGivenEvent evt(d);
        MessageUtils::applyEvent<Aggregate, InitiativeSupportGivenEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "INITIATIVE_SUPPORT_REVOKED_EVENT") {
        JsonUtils::validate(InitiativeSupportRevokedEvent::schema, d);
        const InitiativeSupportRevokedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, InitiativeSupportRevokedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_ADMISSION_QUORUM_CONTINUED_EVENT") {
        JsonUtils::validate(IssueAdmissionQuorumContinuedEvent::schema, d);
        const IssueAdmissionQuorumContinuedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueAdmissionQuorumContinuedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_ADMISSION_QUORUM_FAILED_EVENT") {
        JsonUtils::validate(IssueAdmissionQuorumFailedEvent::schema, d);
        const IssueAdmissionQuorumFailedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueAdmissionQuorumFailedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_ADMISSION_QUORUM_PASSED_EVENT") {
        JsonUtils::validate(IssueAdmissionQuorumPassedEvent::schema, d);
        const IssueAdmissionQuorumPassedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueAdmissionQuorumPassedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_BALLOT_SET_EVENT") {
        JsonUtils::validate(IssueBallotSetEvent::schema, d);
        const IssueBallotSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueBallotSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_BALLOT_UNSET_EVENT") {
        JsonUtils::validate(IssueBallotUnsetEvent::schema, d);
        const IssueBallotUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueBallotUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_DELEGATION_SET_EVENT") {
        JsonUtils::validate(IssueDelegationSetEvent::schema, d);
        const IssueDelegationSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueDelegationSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_DELEGATION_UNSET_EVENT") {
        JsonUtils::validate(IssueDelegationUnsetEvent::schema, d);
        const IssueDelegationUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueDelegationUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_DISCUSSION_PHASE_COMPLETED_EVENT") {
        JsonUtils::validate(IssueDiscussionPhaseCompletedEvent::schema, d);
        const IssueDiscussionPhaseCompletedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueDiscussionPhaseCompletedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT") {
        JsonUtils::validate(IssueVerificationPhaseCompletedEvent::schema, d);
        const IssueVerificationPhaseCompletedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueVerificationPhaseCompletedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "ISSUE_VOTING_PHASE_COMPLETED_EVENT") {
        JsonUtils::validate(IssueVotingPhaseCompletedEvent::schema, d);
        const IssueVotingPhaseCompletedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, IssueVotingPhaseCompletedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "NEW_INITIATIVE_CREATED_EVENT") {
        JsonUtils::validate(NewInitiativeCreatedEvent::schema, d);
        const NewInitiativeCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, NewInitiativeCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "PRIVILEGE_GRANTED_EVENT") {
        JsonUtils::validate(PrivilegeGrantedEvent::schema, d);
        const PrivilegeGrantedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, PrivilegeGrantedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_DELEGATION_SET_EVENT") {
        JsonUtils::validate(UnitDelegationSetEvent::schema, d);
        const UnitDelegationSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitDelegationSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_DELEGATION_UNSET_EVENT") {
        JsonUtils::validate(UnitDelegationUnsetEvent::schema, d);
        const UnitDelegationUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitDelegationUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_POLICY_SET_EVENT") {
        JsonUtils::validate(UnitPolicySetEvent::schema, d);
        const UnitPolicySetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitPolicySetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      throw CommandEvaluationException(
          ExceptionCode::GENERAL_PROJECT_EXCEPTION,
          "There is no ifBlock for " + msgType + ". Run make again, and check the generateEventUtils script");
    }

  };
}


  