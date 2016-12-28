#include <stdexcept>
#include "MemberWeightCalculatorImpl.h"
#include "framework/IdToolsImpl.h"



unsigned long long rooset::MemberWeightCalculatorImpl::calcSupportWeight(
    const uuid& trusteeId,
    const Initiative& initiative,
    const IssueAggregate& issue,
    const Area& area,
    const UnitAggregate& unit)
{
  return calcIssueSupportWeight(
      trusteeId, getMemberStartingWeight(unit, trusteeId),
      initiative, issue, area, unit);
}



unsigned long long rooset::MemberWeightCalculatorImpl::calcIssueSupportWeight(
    const uuid& trusteeId,
    const unsigned long long inWeight,
    const Initiative& initiative,
    const IssueAggregate& issue,
    const Area& area,
    const UnitAggregate& unit)
{
  IdToolsImpl idTools;
  auto weight = inWeight;
  const auto delegations = issue.getDelegations();
  for (auto i = delegations.begin(); i != delegations.end(); ++i) {
    auto trusterId = i->first;
    auto delegationTrusteeId = i->second;
    if (delegationTrusteeId == trusteeId &&
        !isMemberSupportingInitiativeDirectly(initiative.supporters, trusterId)) {
      weight = weight + calcIssueSupportWeight(
        trusterId, getMemberStartingWeight(unit, trusterId),
        initiative, issue, area, unit);
    }
  }
  return calcAreaSupportWeight(trusteeId, weight, initiative,
    issue, area, unit);
}



unsigned long long rooset::MemberWeightCalculatorImpl::calcAreaSupportWeight(
  const uuid& trusteeId,
  const unsigned long long inWeight,
  const Initiative& initiative,
  const IssueAggregate& issue,
  const Area& area,
  const UnitAggregate& unit)
{
  auto weight = inWeight;
  const auto delegations = area.delegations;
  for (auto it = delegations.begin(); it != delegations.end(); it++) {
    auto trusterId = it->first;
    auto delegationTrusteeId = it->second;
    if (delegationTrusteeId == trusteeId &&
        !isMemberSupportingInitiativeDirectly(initiative.supporters, trusterId) &&
        !isDelegated(issue.getDelegations(), trusterId) &&
        !isDelegationBlocked(issue.getBlockedDelegations(), trusterId)) {
      weight = weight + calcIssueSupportWeight(trusterId, getMemberStartingWeight(unit, trusterId),
          initiative, issue, area, unit);
    }
  }
  IdToolsImpl idTools;
  return calcUnitSupportWeight(trusteeId, weight, initiative,
    issue, area, unit);
}



unsigned long long rooset::MemberWeightCalculatorImpl::calcUnitSupportWeight(
  const uuid& trusteeId,
  const unsigned long long inWeight,
  const Initiative& initiative,
  const IssueAggregate& issue,
  const Area& area,
  const UnitAggregate& unit)
{
  auto weight = inWeight;
  const auto delegations = unit.getDelegations();
  for (auto it = delegations.begin(); it != delegations.end(); it++) {
    auto trusterId = it->first;
    auto delegationTrusteeId = it->second;
    if (delegationTrusteeId == trusteeId &&
        !isMemberSupportingInitiativeDirectly(initiative.supporters, trusterId) &&
        !isDelegated(issue.getDelegations(), trusterId) &&
        !isDelegationBlocked(issue.getBlockedDelegations(), trusterId) &&
        !isDelegated(area.delegations, trusterId) &&
        !isDelegationBlocked(area.blockedDelegations, trusterId)) {
      weight = weight + calcIssueSupportWeight(trusterId, getMemberStartingWeight(unit, trusterId),
          initiative, issue, area, unit);
    }
  }
  IdToolsImpl idTools;
  return weight;
}



unsigned long long rooset::MemberWeightCalculatorImpl::getMemberStartingWeight(
  const UnitAggregate& unit, const uuid& memberId)
{
  try {
    auto memberPrivilege = unit.getPrivileges().at(memberId);
    return memberPrivilege.weight;
  } catch (const out_of_range& e) {
    // if member has had their privilege revoked, their vote doesn't count
    return 0;
  }
}



bool rooset::MemberWeightCalculatorImpl::isMemberSupportingInitiativeDirectly(
  const vector<uuid>& supporters, const uuid& trusterId)
{
  auto it = find(supporters.begin(), supporters.end(), trusterId);
  return it != supporters.end();
}



bool rooset::MemberWeightCalculatorImpl::isDelegationBlocked(
    const vector<uuid> blockedDelegations, const uuid& trusterId)
{
  auto it = find(blockedDelegations.begin(), blockedDelegations.end(), trusterId);
  return it != blockedDelegations.end();
}



bool rooset::MemberWeightCalculatorImpl::isDelegated(
    const map<uuid, uuid>& delegations, const uuid& trusterId)
{
  auto it = delegations.find(trusterId);
  return it != delegations.end();
}




