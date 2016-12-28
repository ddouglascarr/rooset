#include <set>
#include <vector>
#include "IssueAggregate.h"

using namespace std;
namespace rooset {

  class VoteCalculator
  {
  public:
    virtual ~VoteCalculator() {};
    virtual set<int> calcWinners(
        vector<vector<vector<unsigned long long>>>& strogestPathMatrix,
        vector<vector<int>>& winningPairs,
        const vector<vector<unsigned long long>>& pairwiseMatrix) = 0;
  };

}
