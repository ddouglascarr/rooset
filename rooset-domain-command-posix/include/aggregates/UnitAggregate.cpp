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
      e.pollingRight, e.votingRight, e.initiativeRight, e.managementRight, e.weight };
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
  policies[e.policyId] = {
    e.name,
    true,
    e.issueQuorumNum,
    e.issueQuorumDen,
  };
}


void rooset::UnitAggregate::handleEvent(const ConcernCreatedEvent& e)
{
  Concern concern { 
    true, 
    e.initiativeContentType,
    e.config 
  };
  concerns[e.concernId] = concern;
}


void rooset::UnitAggregate::handleEvent(const ConcernDeactivatedEvent& e)
{
  auto concern = concerns.at(e.concernId);
  concern.active = false;
  concerns[e.concernId] = concern;
}



void rooset::UnitAggregate::handleEvent(const AreaConcernAddedEvent& e)
{
  auto area = areas.at(e.areaId);
  area.concerns.push_back(e.concernId);
  areas[e.areaId] = area;
}



void rooset::UnitAggregate::handleEvent(const AreaConcernRemovedEvent& e)
{
  auto area = areas.at(e.areaId);
  auto it = find(area.concerns.begin(), area.concerns.end(), e.concernId);
  if (it != area.concerns.end()) {
    area.concerns.erase(it);
  }
  areas[e.areaId] = area;
}
