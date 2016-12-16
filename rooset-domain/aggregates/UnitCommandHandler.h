#pragma once
#include <memory>
#include <stdexcept>
#include "framework/uuid.h"
#include "framework/AggregateRepository.h"
#include "framework/ProjectEvent.h"
#include "exceptions/CommandEvaluationException.h"
#include "enums/ExceptionCode.h"
#include "UnitAggregate.h"
#include "commands/GrantPrivilegeCommand.h"
#include "events/PrivilegeGrantedEvent.h"
#include "commands/CreateAreaCommand.h"
#include "events/AreaCreatedEvent.h"
#include "commands/SetUnitDelegationCommand.h"
#include "events/UnitDelegationSetEvent.h"
#include "commands/UnsetUnitDelegationCommand.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "commands/SetAreaDelegationCommand.h"
#include "events/AreaDelegationSetEvent.h"
#include "commands/UnsetAreaDelegationCommand.h"
#include "events/AreaDelegationUnsetEvent.h"
#include "commands/SetUnitPolicyCommand.h"
#include "events/UnitPolicySetEvent.h"


namespace rooset {

  class UnitCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<UnitAggregate>> repository;
    uuid getUnitDelegation(const UnitAggregate& unit, const uuid& memberId) const;
    uuid getAreaDelegation(
        const UnitAggregate& unit, const uuid& areaId, const uuid& trusterId) const;

  public:
    inline UnitCommandHandler(unique_ptr<AggregateRepository<UnitAggregate>> r) :
      repository(move(r))
    { }

    unique_ptr<ProjectEvent<Document>> evaluate(const GrantPrivilegeCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const CreateAreaCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const SetUnitDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const UnsetUnitDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const SetAreaDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const UnsetAreaDelegationCommand& c);
    unique_ptr<ProjectEvent<Document>> evaluate(const SetUnitPolicyCommand& c);
  };
}