#include <set>
#include <vector>
#include "IssueAggregate.h"
#include "framework/uuid.h"

using namespace std;
namespace rooset {

  class VoteCalculator
  {
  public:
    class SchulzeBallot {
    private:
      IdToolsImpl idTools;
      vector<vector<uuid>> schulzeRanking;
      unsigned long long weight = 0;
    public:
      SchulzeBallot(
          const vector<vector<uuid>>& approve,
          const vector<uuid>& abstain,
          const vector<vector<uuid>>& disapprove);

      inline auto getSchulzeRanking() const { return schulzeRanking; }
      inline void setWeight(unsigned long long w) { weight = w; }
      inline auto getWeight() const { return weight; }
    };

    virtual ~VoteCalculator() {};

    virtual set<uuid> calcWinners(
      const vector<SchulzeBallot>& ballots,
      const vector<uuid>& initiativeIds) =0;

  };

}
