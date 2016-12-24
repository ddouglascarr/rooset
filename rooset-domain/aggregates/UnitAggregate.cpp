#include <vector>
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
  auto& area = areas.at(e.areaId);
  area.delegations[e.trusterId] = e.trusteeId;
  // remove block if present
  auto i = find(area.blockedDelegations.begin(), area.blockedDelegations.end(), e.trusterId);
  if (i != area.blockedDelegations.end()) area.blockedDelegations.erase(i);
}

void rooset::UnitAggregate::handleEvent(const AreaDelegationUnsetEvent & e)
{
  auto& areaDelegations = areas.at(e.areaId).delegations;
  auto it = areaDelegations.find(e.trusterId);
  if (it != areaDelegations.end()) {
    areaDelegations.erase(it);
  }
}

void rooset::UnitAggregate::handleEvent(const DelegationBlockedForAreaEvent& e)
{
  auto& area = areas.at(e.areaId);
  area.blockedDelegations.push_back(e.trusterId);
  // remove delegation if present
  auto i = area.delegations.find(e.trusterId);
  if (i != area.delegations.end()) {
    area.delegations.erase(i);
  }
}

void rooset::UnitAggregate::handleEvent(const DelegationUnblockedForAreaEvent& e)
{
  auto& blockedAreaDelegations = areas.at(e.areaId).blockedDelegations;
  auto it = find(blockedAreaDelegations.begin(), blockedAreaDelegations.end(), e.trusterId);
  if (it != blockedAreaDelegations.end()) {
    blockedAreaDelegations.erase(it);
  }
}

void rooset::UnitAggregate::handleEvent(const UnitPolicySetEvent & e)
{
  Policy policy{ e.name, true };
  policies[e.policyId] = policy;
}
