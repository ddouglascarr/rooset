#include "IssueCommandHandler.h"
#include <stdexcept>
#include "IssueCommandHandler.h"
#include "exceptions/CommandEvaluationException.h"
#include "PrivilegeUtils.h"

unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(const CreateNewInitiativeCommand& c)
{
  issueRepository->assertAggregateDoesNotExist(c.id);
  cout << "assert should have thrown\n";
  auto unit = unitRepository->load(c.unitId);
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  try {
    unit->getAreas().at(c.areaId);
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
        "That area is not part of this unit");
  }
  return unique_ptr<NewInitiativeCreatedEvent>(new NewInitiativeCreatedEvent(c));
}

unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(const CreateCompetingInitiativeCommand & c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  try {
    issue->getInitiatives().at(c.initiativeId);
    throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
        "An initiative by that id already exists");
  } catch (const std::out_of_range& e) {
    // good, should not be there
  }
  return unique_ptr<CompetingInitiativeCreatedEvent>(
      new CompetingInitiativeCreatedEvent(c));
}
