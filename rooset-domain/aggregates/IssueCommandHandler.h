#pragma once
#include <memory>
#include <vector>
#include "framework/uuid.h"
#include "framework/AggregateRepository.h"
#include "UnitAggregate.h"
#include "IssueAggregate.h"
#include "enums/IssueState.h"
#include "DelegationCalculatorImpl.h"
#include "commands/CreateNewInitiativeCommand.h"
#include "commands/CreateCompetingInitiativeCommand.h"
#include "commands/SetIssueDelegationCommand.h"
#include "commands/UnsetIssueDelegationCommand.h"
#include "commands/BlockDelegationForIssueCommand.h"
#include "commands/UnblockDelegationForIssueCommand.h"
#include "commands/GiveInitiativeSupportCommand.h"
#include "commands/RevokeInitiativeSupportCommand.h"
#include "commands/AssessIssueAdmissionQuorumCommand.h"
#include "commands/CompleteIssueAdmissionPhaseCommand.h"
#include "commands/CompleteIssueVotingPhaseCommand.h"
#include "commands/SetIssueBallotCommand.h"
#include "commands/UnsetIssueBallotCommand.h"
#include "VoteCalculatorSchulzeImpl.h"


using namespace std;

namespace rooset {

  class IssueCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<IssueAggregate>> issueRepository;
    unique_ptr<AggregateRepository<UnitAggregate>> unitRepository;
    unique_ptr<DelegationCalculator> delegationCalculator = make_unique<
        DelegationCalculatorImpl>();
    unique_ptr<VoteCalculator> voteCalculator = make_unique<
        VoteCalculatorSchulzeImpl>();
    unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

  public:
    inline IssueCommandHandler(
      unique_ptr<AggregateRepository<IssueAggregate>> issueRepository,
      unique_ptr<AggregateRepository<UnitAggregate>> unitRepository) :
      issueRepository(move(issueRepository)),
      unitRepository(move(unitRepository))
    { }

    unique_ptr<ProjectEvent<Document>> evaluate(const CreateNewInitiativeCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const CreateCompetingInitiativeCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const SetIssueDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const UnsetIssueDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const BlockDelegationForIssueCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const UnblockDelegationForIssueCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const GiveInitiativeSupportCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const RevokeInitiativeSupportCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const AssessIssueAdmissionQuorumCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const CompleteIssueAdmissionPhaseCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const SetIssueBallotCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const UnsetIssueBallotCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const CompleteIssueVotingPhaseCommand& c);

  protected:
    void assertIssueState(const IssueAggregate& issue, const vector<IssueState>& acceptable);
    bool isAdmissionQuorumPassed(const IssueAggregate& issue, const UnitAggregate& unit);
    bool isAdmissionQuorumPassed(
        const rooset::Policy& policy,
        const unsigned long long totalVoteWeight,
        const unsigned long long support);

    static unsigned long long calcTotalVoteWeight(
        const map<uuid, rooset::MemberPrivilege>& privileges);
  };

}
