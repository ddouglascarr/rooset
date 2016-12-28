#include <stdexcept>
#include "VoteUtils.h"
#include "framework/IdToolsImpl.h"



unsigned long long rooset::VoteUtils::calcSupportWeight(
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



unsigned long long rooset::VoteUtils::calcTotalVoteWeight(
    const map<uuid, rooset::MemberPrivilege>& privileges)
{
  auto totalVoteWeight = 0;
  for (auto it : privileges) {
    const auto privilege = it.second;
    if (privilege.votingRight) {
      totalVoteWeight += privilege.weight;
    }
  }
  return totalVoteWeight;
}



bool rooset::VoteUtils::isAdmissionQuorumPassed(
    const rooset::Policy& policy, 
    const unsigned long long totalVoteWeight,
    const unsigned long long support)
{
  return (support * policy.issueQuorumDen) >= (policy.issueQuorumNum * totalVoteWeight);
}



vector<vector<unsigned long long>> rooset::VoteUtils::calcPairwiseMatrix(
    const vector<SchulzeBallot>& ballots,
    const vector<uuid>& initiativeIds)
{
  vector<vector<unsigned long long>> matrix(initiativeIds.size(), vector<unsigned long long>(
      initiativeIds.size(), 0));
  
  map<uuid, int> idPos;
  for (int i = 0; i < initiativeIds.size(); ++i) {
    idPos[initiativeIds[i]] = i;
  }

  for (auto ballot : ballots) {
    auto schulzeRanking = ballot.getSchulzeRanking();
    for (int iRank = 0; iRank < schulzeRanking.size(); ++iRank) {
      for (uuid iId : schulzeRanking[iRank]) {
        for (int jRank = 0; jRank < schulzeRanking.size(); ++jRank) {
          for (uuid jId : schulzeRanking[jRank]) {
            if (iRank < jRank) {
              matrix[idPos[iId]][idPos[jId]] += ballot.getWeight();
            }
          }
        }
      }
    }
  }

  return matrix;
}



/*
 * Described in S2.3 of Schulze (draft, 23 December 2016) (pp.12-14)
 * at http://m-schulze.9mail.de/schulze1.pdf
 */
set<int> rooset::VoteUtils::calcWinners(
    vector<vector<vector<unsigned long long>>>& strogestPathMatrix,
    vector<vector<int>>& winningPairs,
    const vector<vector<unsigned long long>>& pairwiseMatrix)
{
  set<int> winners;
  const int mSize = pairwiseMatrix.size();
  auto& n = pairwiseMatrix;
  auto& pd = strogestPathMatrix;
  vector<vector<int>> pred(mSize, vector<int>(mSize, 0));  
 
  strogestPathMatrix = vector<vector<vector<unsigned long long>>>(mSize,
      vector<vector<unsigned long long>>(mSize, vector<unsigned long long>(2, 0)));
  
  // Stage 1 (initialization)
  for (int i = 0; i < mSize; ++i) {
    for (int j = 0; j < mSize; ++j) {
      if (i == j) continue;
      pd[i][j] = { n[i][j], n[j][i] };
      pred[i][j] = i;
    }
  }

  // Stage 2 (calculation of the stengths of the strongest paths)
  for (int i = 0; i < mSize; ++i) {
    for (int j = 0; j < mSize; ++j) {
      if (i == j) continue;
      for (int k = 0; k < mSize; ++k) {
        if (j == k) continue;
        auto smallestJik = comparePairwiseBattle(pd[j][i], pd[i][k]) == -1 ?
            pd[j][i] : pd[i][k];
        bool isJkWeakerThanJik = comparePairwiseBattle(pd[j][k], smallestJik) == -1;
        if (isJkWeakerThanJik) {
          pd[j][k] = smallestJik;          
          if (pred[j][k] != pred[i][k]) {
            pred[j][k] = pred[i][k];
          }
        }
      }
    }
  }

  // Stage 3 (calculation of the binary relation O and the winners
  for (int i = 0; i < mSize; ++i) {
    winners.insert(i);
    for (int j = 0; j < mSize; ++j) {
      if (i == j) continue;
      bool isJiStrongerThanIj = comparePairwiseBattle(pd[j][i], pd[i][j]) == 1;
      if (isJiStrongerThanIj) {
        winningPairs.push_back({j, i});
        winners.erase(i);
      }
    }
  }

  return winners;
}



int rooset::VoteUtils::comparePairwiseBattle(
    const vector<unsigned long long> a,
    const vector<unsigned long long> b)
{
  if (a.at(0) > b.at(0)) return 1; 
  if (a.at(0) < b.at(0)) return -1;
  if (b.at(1) > a.at(1)) return 1;
  if (b.at(1) < a.at(1)) return -1;
  return 0;
}



unsigned long long rooset::VoteUtils::calcIssueSupportWeight(
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



unsigned long long rooset::VoteUtils::calcAreaSupportWeight(
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



unsigned long long rooset::VoteUtils::calcUnitSupportWeight(
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



unsigned long long rooset::VoteUtils::getMemberStartingWeight(
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




