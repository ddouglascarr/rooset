#include "IssueAggregate.h"

rooset::IssueAggregate::IssueAggregate(const NewInitiativeCreatedEvent& e) :
    id(e.id), unitId(e.unitId), areaId(e.areaId)
{
  initiatives[e.initiativeId] = Initiative{ e.name, e.requesterId };
}

void rooset::IssueAggregate::handleEvent(const CompetingInitiativeCreatedEvent & e)
{
  initiatives[e.initiativeId] = Initiative{ e.name, e.requesterId };
}

void rooset::IssueAggregate::handleEvent(const IssueDelegationSetEvent& e)
{
  delegations[e.trusterId] = e.trusteeId;
}

void rooset::IssueAggregate::handleEvent(const IssueDelegationUnsetEvent& e)
{
  delegations.erase(e.trusterId);
}