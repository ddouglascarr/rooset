#pragma once
#include <string>
#include <map>
#include "framework/IdToolsImpl.h"
#include "framework/uuid.h"
#include "enums/IssueState.h"
#include "events/NewInitiativeCreatedEvent.h"
#include "events/CompetingInitiativeCreatedEvent.h"
#include "events/IssueDelegationSetEvent.h"
#include "events/IssueDelegationUnsetEvent.h"
#include "events/DelegationBlockedForIssueEvent.h"
#include "events/DelegationUnblockedForIssueEvent.h"
#include "events/IssueAdmissionQuorumPassedEvent.h"
#include "events/IssueAdmissionQuorumFailedEvent.h"
#include "events/IssueAdmissionQuorumContinuedEvent.h"
#include "events/IssueDiscussionPhaseCompletedEvent.h"
#include "events/IssueVerificationPhaseCompletedEvent.h"
#include "events/IssueVotingPhaseCompletedEvent.h"
#include "events/InitiativeSupportGivenEvent.h"
#include "events/InitiativeSupportRevokedEvent.h"

using namespace std;

namespace rooset {

  struct Initiative {
    string name;
    uuid initiatorId;
    vector<uuid> supporters;
  };

  class IssueAggregate
  {
  private:
    const uuid id;
    const uuid unitId;
    const uuid areaId;
    const uuid policyId;

    IssueState issueState;
    map<uuid, Initiative> initiatives;
    map<uuid, uuid> delegations;
    vector<uuid> blockedDelegations;

  public:
    IssueAggregate(const NewInitiativeCreatedEvent& e);
    void handleEvent(const CompetingInitiativeCreatedEvent& e);
    void handleEvent(const IssueDelegationSetEvent& e);
    void handleEvent(const IssueDelegationUnsetEvent& e);
    void handleEvent(const DelegationBlockedForIssueEvent& e);
    void handleEvent(const DelegationUnblockedForIssueEvent& e);
    void handleEvent(const InitiativeSupportGivenEvent& e);
    void handleEvent(const InitiativeSupportRevokedEvent& e);

    void handleEvent(const IssueAdmissionQuorumPassedEvent& e);
    void handleEvent(const IssueAdmissionQuorumFailedEvent& e);
    void handleEvent(const IssueDiscussionPhaseCompletedEvent& e);
    void handleEvent(const IssueVerificationPhaseCompletedEvent& e);
    void handleEvent(const IssueVotingPhaseCompletedEvent& e);


    inline uuid getId() const { return id; }
    inline uuid getUnitId() const { return unitId; }
    inline uuid getAreaId() const { return areaId; }
    inline uuid getPolicyId() const { return policyId; }
    inline auto getIssueState() const { return issueState; }
    inline auto getInitiatives() const { return initiatives; }
    inline auto getDelegations() const { return delegations; }
    inline auto getBlockedDelegations() const { return blockedDelegations; }
  };
}
