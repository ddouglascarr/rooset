#pragma once
#include "VoteCalculator.h"
#include "ratk/IdToolsImpl.h"

using namespace std;
namespace rooset {

  class VoteCalculatorSchulzeImpl : public VoteCalculator
  {
  public:
    virtual set<uuid> calcWinners(
        const vector<SchulzeBallot>& ballots,
        const map<uuid, Initiative>& initiatives) override;

  protected:
    unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    set<uuid> calcWinners(
      const vector<SchulzeBallot>& ballots,
      const vector<uuid>& initiativeIds);

    set<int> calcSchulzeWinners(
        vector<vector<vector<unsigned long long>>>& strogestPathMatrix,
        vector<vector<int>>& winningPairs,
        const vector<vector<unsigned long long>>& pairwiseMatrix);

    vector<vector<unsigned long long>> calcPairwiseMatrix(
        const vector<SchulzeBallot>& ballots,
        const vector<uuid>& initiativeIds);
    
    int comparePairwiseBattle(
        const vector<unsigned long long> a,
        const vector<unsigned long long> b);

  };

}
