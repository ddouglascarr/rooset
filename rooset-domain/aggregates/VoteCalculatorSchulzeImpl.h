#include "VoteCalculator.h"

using namespace std;
namespace rooset {

  class VoteCalculatorSchulzeImpl : public VoteCalculator
  {
  public:
    set<uuid> calcWinners(
      const vector<SchulzeBallot>& ballots,
      const vector<uuid>& initiativeIds) override;

  protected:
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
