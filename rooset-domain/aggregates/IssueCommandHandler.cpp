#include <stdexcept>
#include "IssueCommandHandler.h"
#include "exceptions/CommandEvaluationException.h"
#include "PrivilegeUtils.h"

unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(const CreateNewInitiativeCommand& c)
{
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