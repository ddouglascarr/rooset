#include "UnitCommandHandler.h"

void rooset::UnitCommandHandler::assertManagementRight(const UnitAggregate& unit, const uuid& memberId)
{
  try {
    const auto requesterPrivileges = unit.getPrivileges().at(memberId);
    if (!requesterPrivileges.managementRight) {
      throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member does not have management rights");
    }
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member is not privilgegd for this unit");
  }
}

void rooset::UnitCommandHandler::assertVotingRight(const UnitAggregate & unit, const uuid & memberId)
{
  try {
    const auto requesterPrivileges = unit.getPrivileges().at(memberId);
    if (!requesterPrivileges.votingRight) {
      throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member does not have voting rights");
    }
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member is not privilgegd for this unit");
  }
}

uuid rooset::UnitCommandHandler::getUnitDelegation(const UnitAggregate& unit, const uuid& memberId) const
{
  try {
    return unit.getDelegations().at(memberId);
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
      "unit delegation does not exist");
  }
}

uuid rooset::UnitCommandHandler::getAreaDelegation(
    const UnitAggregate & unit, const uuid& areaId, const uuid & trusterId) const
{
  try {
    return unit.getAreas().at(areaId).delegations.at(trusterId);
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
      "Area delegation does not exist");
  }
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const GrantPrivilegeCommand& c)
{
  const auto unit = repository->load(c.id);
  assertManagementRight(*unit, c.requesterId);
  return unique_ptr<PrivilegeGrantedEvent>(new PrivilegeGrantedEvent(
    c.id, c.requesterId, c.memberId, c.pollingRight, c.votingRight,
    c.initiativeRight, c.managementRight));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const CreateAreaCommand & c)
{
  const auto unit = repository->load(c.id);
  assertManagementRight(*unit, c.requesterId);

  const auto areas = unit->getAreas();
  const bool isAreaPresent = areas.find(c.areaId) != areas.end();
  if (isAreaPresent) {
    throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
      "Area already exists");
  }
  
  return unique_ptr<AreaCreatedEvent>(new AreaCreatedEvent(
    c.id, c.areaId, c.requesterId, c.name, c.description, c.externalReference));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const SetUnitDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  const auto unit = repository->load(c.id);
  assertVotingRight(*unit, trusterId);
  assertVotingRight(*unit, c.trusteeId);

  return unique_ptr<UnitDelegationSetEvent>(new UnitDelegationSetEvent(
    c.id, trusterId, c.trusteeId));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const UnsetUnitDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  const auto unit = repository->load(c.id);
  getUnitDelegation(*unit, trusterId); // throws if missing
  return unique_ptr<UnitDelegationUnsetEvent>(new UnitDelegationUnsetEvent(
    c.id, trusterId));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const SetAreaDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  try {
    const auto unit = repository->load(c.id);
    unit->getAreas().at(c.areaId); // check area exists
    assertVotingRight(*unit, trusterId);
    assertVotingRight(*unit, c.trusteeId);

    return unique_ptr<AreaDelegationSetEvent>(new AreaDelegationSetEvent(
      c.id, c.areaId, trusterId, c.trusteeId));
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
      "Area does not exist");
  }
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const UnsetAreaDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  const auto unit = repository->load(c.id);
  getAreaDelegation(*unit, c.areaId, trusterId); // throws if not there
  return unique_ptr<AreaDelegationUnsetEvent>(new AreaDelegationUnsetEvent(
    c.id, c.areaId, trusterId));
}