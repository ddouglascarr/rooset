#pragma once
#include <string>
#include <map>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o
#include "ratk/IdToolsImpl.h"
#include "ratk/uuid.h"
#include "enums/IssueState.h"
#include "aggregates/SchulzeBallot.h"
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
#include "events/IssueBallotSetEvent.h"
#include "events/IssueBallotUnsetEvent.h"


using namespace std;

namespace rooset {

  struct Initiative {
    string name;
    uuid initiatorId;
    boost::posix_time::ptime created;
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
    map<uuid, SchulzeBallot> ballots;

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
    void handleEvent(const IssueBallotSetEvent& e);
    void handleEvent(const IssueBallotUnsetEvent& e);


    inline uuid getId() const { return id; }
    inline uuid getUnitId() const { return unitId; }
    inline uuid getAreaId() const { return areaId; }
    inline uuid getPolicyId() const { return policyId; }
    inline auto getIssueState() const { return issueState; }
    inline auto getInitiatives() const { return initiatives; }
    inline auto getDelegations() const { return delegations; }
    inline auto getBlockedDelegations() const { return blockedDelegations; }
    inline auto getBallots() const { return ballots; }
  };
}
