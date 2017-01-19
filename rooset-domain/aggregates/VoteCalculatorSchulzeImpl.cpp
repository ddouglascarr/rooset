#include <stdexcept>
#include <memory>
#include "VoteCalculatorSchulzeImpl.h"
#include "framework/IdToolsImpl.h"



set<uuid> rooset::VoteCalculatorSchulzeImpl::calcWinners(
        const vector<SchulzeBallot>& ballots,
        const map<uuid, Initiative>& initiatives)
{
  vector<uuid> initiativeIds = { idTools->generateNilId() }; // status quo
  for (auto it = initiatives.begin(); it != initiatives.end(); ++it) {
    initiativeIds.push_back(it->first);
  }
  return calcWinners(ballots, initiativeIds);
}



set<uuid> rooset::VoteCalculatorSchulzeImpl::calcWinners(
    const vector<SchulzeBallot>& ballots,
    const vector<uuid>& initiativeIds)
{
  auto pairwiseMatrix = calcPairwiseMatrix(ballots, initiativeIds);
  vector<vector<vector<unsigned long long>>> strongestPathMatrix;
  vector<vector<int>> winningPairs;

  auto schulzeWinners = calcSchulzeWinners(
    strongestPathMatrix, winningPairs, pairwiseMatrix);

  set<uuid> winners;
  for (auto i : schulzeWinners) {
    winners.insert(initiativeIds.at(i));
  }
  return winners;
}



/*
 * Described in S2.3 of Schulze (draft, 23 December 2016) (pp.12-14)
 * at http://m-schulze.9mail.de/schulze1.pdf
 */
set<int> rooset::VoteCalculatorSchulzeImpl::calcSchulzeWinners(
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



vector<vector<unsigned long long>> rooset::VoteCalculatorSchulzeImpl::calcPairwiseMatrix(
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



int rooset::VoteCalculatorSchulzeImpl::comparePairwiseBattle(
    const vector<unsigned long long> a,
    const vector<unsigned long long> b)
{
  if (a.at(0) > b.at(0)) return 1; 
  if (a.at(0) < b.at(0)) return -1;
  if (b.at(1) > a.at(1)) return 1;
  if (b.at(1) < a.at(1)) return -1;
  return 0;
}


