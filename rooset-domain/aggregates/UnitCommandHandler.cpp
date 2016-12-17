#include "UnitCommandHandler.h"
#include "PrivilegeUtils.h"
#include "CommandHandlerUtils.h"

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
  PrivilegeUtils::assertManagementRight(*unit, c.requesterId);
  return unique_ptr<PrivilegeGrantedEvent>(new PrivilegeGrantedEvent(
    c.id, c.requesterId, c.memberId, c.pollingRight, c.votingRight,
    c.initiativeRight, c.managementRight));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const CreateAreaCommand & c)
{
  const auto unit = repository->load(c.id);
  PrivilegeUtils::assertManagementRight(*unit, c.requesterId);
  CommandHandlerUtils::assertMapExcludes<decltype(unit->getAreas()), uuid>(
      unit->getAreas(), c.areaId, "Area already exists");
  
  return unique_ptr<AreaCreatedEvent>(new AreaCreatedEvent(
    c.id, c.areaId, c.requesterId, c.name, c.description, c.externalReference));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const SetUnitDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  const auto unit = repository->load(c.id);
  PrivilegeUtils::assertVotingRight(*unit, trusterId);
  PrivilegeUtils::assertVotingRight(*unit, c.trusteeId);

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
  const auto unit = repository->load(c.id);
  CommandHandlerUtils::assertMapContains<decltype(unit->getAreas()), uuid>(
      unit->getAreas(), c.areaId, "Area does not exists");
  PrivilegeUtils::assertVotingRight(*unit, trusterId);
  PrivilegeUtils::assertVotingRight(*unit, c.trusteeId);

  return unique_ptr<AreaDelegationSetEvent>(new AreaDelegationSetEvent(
      c.id, c.areaId, trusterId, c.trusteeId));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const UnsetAreaDelegationCommand & c)
{
  const auto trusterId = c.requesterId;
  const auto unit = repository->load(c.id);
  getAreaDelegation(*unit, c.areaId, trusterId); // throws if not there
  return unique_ptr<AreaDelegationUnsetEvent>(new AreaDelegationUnsetEvent(
    c.id, c.areaId, trusterId));
}

unique_ptr<ProjectEvent<Document>> rooset::UnitCommandHandler::evaluate(const SetUnitPolicyCommand& c)
{
  auto unit = repository->load(c.id);
  PrivilegeUtils::assertManagementRight(*unit, c.requesterId);
  return unique_ptr<UnitPolicySetEvent>(new UnitPolicySetEvent(c));
}