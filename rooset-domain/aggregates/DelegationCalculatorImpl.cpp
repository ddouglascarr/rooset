#include <stdexcept>
#include "DelegationCalculatorImpl.h"
#include "framework/IdToolsImpl.h"



map<uuid, uuid> rooset::DelegationCalculatorImpl::calcInitiativeDelegations(
    const Initiative& initiative,
    const IssueAggregate& issue,
    const Area& area,
    const UnitAggregate& unit,
    const map<uuid, MemberPrivilege>& memberPrivileges)
{
  map<uuid, uuid> delegations;
  for (auto it = memberPrivileges.begin(); it != memberPrivileges.end(); ++it) {
    auto memberId = it->first;
    auto memberPrivilege = it->second;
    if (!memberPrivilege.votingRight) continue;
    auto delegation = memberId;
    delegation = getUnitDelegation(unit, memberId, delegation);
    delegation = getAreaDelegation(area, memberId, delegation);
    delegation = getIssueDelegation(issue, memberId, delegation);
    if (isMemberSupportingInitiativeDirectly(initiative.supporters, memberId)) {
      delegation = memberId;
    }

    delegations[memberId] = delegation;
  }
  return delegations;
}



map<uuid, uuid> rooset::DelegationCalculatorImpl::calcIssueDelegations(
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit,
        const map<uuid, MemberPrivilege>& memberPrivileges)
{
  map<uuid, uuid> delegations;
  for (auto it = memberPrivileges.begin(); it != memberPrivileges.end(); ++it) {
    auto memberId = it->first;
    auto memberPrivilege = it->second;
    if (!memberPrivilege.votingRight) continue;
    auto delegation = memberId;
    delegation = getUnitDelegation(unit, memberId, delegation);
    delegation = getAreaDelegation(area, memberId, delegation);
    delegation = getIssueDelegation(issue, memberId, delegation);
    if (isMemberVotingOnIssueDirectly(memberId, issue.getBallots())) delegation = memberId;

    delegations[memberId] = delegation;
  }
  return delegations;
}



unsigned long long rooset::DelegationCalculatorImpl::calcMemberWeight(
    const map<uuid, MemberPrivilege>& memberPrivileges,
    const map<uuid, uuid>& delegations,
    const uuid memberId)
{
  // TODO [OPTIMIZATION]: Use boost::multi_index
  unsigned long long weight = memberPrivileges.at(memberId).weight;
  for (auto it = delegations.begin(); it != delegations.end(); ++it) {
    const auto& trusterId = it->first;
    const auto& trusteeId = it->second;
    if (trusteeId != memberId) continue;
    if (trusterId == memberId) continue; // don't count delegations to self
    weight += calcMemberWeight(memberPrivileges, delegations, trusterId);
  }
  return weight;
}



uuid rooset::DelegationCalculatorImpl::getUnitDelegation(
    const UnitAggregate& unit,
    const uuid memberId,
    const uuid parentDelegation)
{
  const auto delegations = unit.getDelegations();

  if (delegations.find(memberId) != delegations.end()) {
    return delegations.at(memberId);
  }
  return parentDelegation;
}



uuid rooset::DelegationCalculatorImpl::getAreaDelegation(
    const Area& area,
    const uuid memberId,
    const uuid parentDelegation)
{
  const auto& delegations = area.delegations;

  if (isDelegationBlocked(area.blockedDelegations, memberId)) {
    return memberId;
  }
  if (delegations.find(memberId) != delegations.end()) {
    return delegations.at(memberId);
  }
  return parentDelegation;
}



uuid rooset::DelegationCalculatorImpl::getIssueDelegation(
    const IssueAggregate& issue,
    const uuid memberId,
    const uuid parentDelegation)
{
  const auto& delegations = issue.getDelegations();

  if (isDelegationBlocked(issue.getBlockedDelegations(), memberId)) {
    return memberId;
  }
  if (delegations.find(memberId) != delegations.end()) {
    return delegations.at(memberId);
  }
  return parentDelegation;
}



bool rooset::DelegationCalculatorImpl::isMemberSupportingInitiativeDirectly(
  const vector<uuid>& supporters, const uuid& trusterId)
{
  auto it = find(supporters.begin(), supporters.end(), trusterId);
  return it != supporters.end();
}



bool rooset::DelegationCalculatorImpl::isDelegationBlocked(
    const vector<uuid> blockedDelegations, const uuid& trusterId)
{
  auto it = find(blockedDelegations.begin(), blockedDelegations.end(), trusterId);
  return it != blockedDelegations.end();
}



bool rooset::DelegationCalculatorImpl::isMemberVotingOnIssueDirectly(
    const uuid& memberId, const map<uuid, SchulzeBallot>& ballots)
{
  auto it = ballots.find(memberId);
  return it != ballots.end();
}
