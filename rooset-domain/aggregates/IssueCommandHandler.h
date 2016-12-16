#pragma once
#include <memory>
#include "framework/uuid.h"
#include "framework/AggregateRepository.h"
#include "UnitAggregate.h"
#include "IssueAggregate.h"
#include "commands/CreateNewInitiativeCommand.h"
#include "events/NewInitiativeCreatedEvent.h"
#include "commands/CreateCompetingInitiativeCommand.h"
#include "events/CompetingInitiativeCreatedEvent.h"

namespace rooset {

  class IssueCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<IssueAggregate>> issueRepository;
    unique_ptr<AggregateRepository<UnitAggregate>> unitRepository;

  public:
    inline IssueCommandHandler(
      unique_ptr<AggregateRepository<IssueAggregate>> issueRepository,
      unique_ptr<AggregateRepository<UnitAggregate>> unitRepository) :
      issueRepository(move(issueRepository)),
      unitRepository(move(unitRepository))
    { }

    unique_ptr<ProjectEvent<Document>> evaluate(const CreateNewInitiativeCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const CreateCompetingInitiativeCommand& c);

  };

}