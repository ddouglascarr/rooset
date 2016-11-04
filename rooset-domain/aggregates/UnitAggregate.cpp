#include "UnitAggregate.h"

rooset::UnitAggregate::UnitAggregate(const UnitCreatedEvent & e)
{
  privileges[e.requesterId] = MemberPrivilege{ false, false, false, true };
}

void rooset::UnitAggregate::handleEvent(const AreaCreatedEvent & e)
{
  areas[e.areaId] = Area{ e.name, map<uuid, uuid>() };
}

void rooset::UnitAggregate::handleEvent(const PrivilegeGrantedEvent & e)
{
  privileges[e.memberId] = MemberPrivilege{ 
      e.pollingRight, e.votingRight, e.initiativeRight, e.managementRight };
}

void rooset::UnitAggregate::handleEvent(const UnitDelegationSetEvent & e)
{
  delegations[e.trusterId] = e.trusteeId;
}

void rooset::UnitAggregate::handleEvent(const UnitDelegationUnsetEvent & e)
{
  auto it = delegations.find(e.trusterId);
  if (it != delegations.end()) {
    delegations.erase(it);
  }
}

void rooset::UnitAggregate::handleEvent(const AreaDelegationSetEvent & e)
{
  areas.at(e.areaId).delegations[e.trusterId] = e.trusteeId;
}

void rooset::UnitAggregate::handleEvent(const AreaDelegationUnsetEvent & e)
{
  auto& areaDelegations = areas.at(e.areaId).delegations;
  auto it = areaDelegations.find(e.trusterId);
  if (it != areaDelegations.end()) {
    areaDelegations.erase(it);
  }
}
