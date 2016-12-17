#include "IssueCommandHandler.h"
#include <stdexcept>
#include "IssueCommandHandler.h"
#include "exceptions/CommandEvaluationException.h"
#include "PrivilegeUtils.h"
#include "CommandHandlerUtils.h"

unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(const CreateNewInitiativeCommand& c)
{
  issueRepository->assertAggregateDoesNotExist(c.id);
  cout << "assert should have thrown\n";
  auto unit = unitRepository->load(c.unitId);
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  CommandHandlerUtils::assertMapContains<decltype(unit->getAreas()), uuid>(
      unit->getAreas(), c.areaId, "Unit does not contain this area");
  return unique_ptr<NewInitiativeCreatedEvent>(new NewInitiativeCreatedEvent(c));
}

unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(const CreateCompetingInitiativeCommand & c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  CommandHandlerUtils::assertMapExcludes<decltype(issue->getInitiatives()), uuid>(
      issue->getInitiatives(), c.initiativeId, "An initiative by that id already exists");
  return unique_ptr<CompetingInitiativeCreatedEvent>(
      new CompetingInitiativeCreatedEvent(c));
}
