#include "VoteCalculator.h"

using namespace std;
namespace rooset {

  class VoteCalculatorSchulzeImpl : public VoteCalculator
  {
  public:
    set<int> calcWinners(
        vector<vector<vector<unsigned long long>>>& strogestPathMatrix,
        vector<vector<int>>& winningPairs,
        const vector<vector<unsigned long long>>& pairwiseMatrix) override;

  protected:
    vector<vector<unsigned long long>> calcPairwiseMatrix(
        const vector<SchulzeBallot>& ballots,
        const vector<uuid>& initiativeIds);
    
    int comparePairwiseBattle(
        const vector<unsigned long long> a,
        const vector<unsigned long long> b);

  };

}
