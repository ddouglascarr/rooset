#include <stdexcept>
#include "VoteUtils.h"
#include "framework/IdToolsImpl.h"

int rooset::VoteUtils::calcSupportWeight(
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

int rooset::VoteUtils::calcTotalVoteWeight(
  const map<uuid, rooset::MemberPrivilege>& privileges)
{
  int totalVoteWeight = 0;
  for (auto it : privileges) {
    const auto privilege = it.second;
    if (privilege.votingRight) {
      totalVoteWeight += privilege.weight;
    }
  }
  return totalVoteWeight;
}

bool rooset::VoteUtils::isAdmissionQuorumPassed(
    const rooset::Policy& policy, const int totalVoteWeight, const int support)
{
  return (support * policy.issueQuorumDen) >= (policy.issueQuorumNum * totalVoteWeight);
}

int rooset::VoteUtils::calcIssueSupportWeight(
  const uuid& trusteeId,
  const int inWeight,
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

int rooset::VoteUtils::calcAreaSupportWeight(
  const uuid& trusteeId,
  const int inWeight,
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

int rooset::VoteUtils::calcUnitSupportWeight(
  const uuid& trusteeId,
  const int inWeight,
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

int rooset::VoteUtils::getMemberStartingWeight(
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

bool rooset::VoteUtils::isMemberSupportingInitiativeDirectly(
  const vector<uuid>& supporters, const uuid& trusterId)
{
  auto it = find(supporters.begin(), supporters.end(), trusterId);
  return it != supporters.end();
}

bool rooset::VoteUtils::isDelegationBlocked(
    const vector<uuid> blockedDelegations, const uuid& trusterId)
{
  auto it = find(blockedDelegations.begin(), blockedDelegations.end(), trusterId);
  return it != blockedDelegations.end();
}

bool rooset::VoteUtils::isDelegated(
    const map<uuid, uuid>& delegations, const uuid& trusterId)
{
  auto it = delegations.find(trusterId);
  return it != delegations.end();
}