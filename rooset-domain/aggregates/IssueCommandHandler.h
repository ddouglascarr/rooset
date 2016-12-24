#pragma once
#include <memory>
#include <vector>
#include "framework/uuid.h"
#include "framework/AggregateRepository.h"
#include "UnitAggregate.h"
#include "IssueAggregate.h"
#include "enums/IssueState.h"
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

using namespace std;

namespace rooset {

  class IssueCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<IssueAggregate>> issueRepository;
    unique_ptr<AggregateRepository<UnitAggregate>> unitRepository;

    void assertIssueState(const IssueAggregate& issue, const vector<IssueState>& acceptable);

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

  };

}