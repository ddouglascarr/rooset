#include "UnitAggregate.h"
#include "IssueAggregate.h"

using namespace std;
namespace rooset {

  class SupportCalculator
  {
  public:
    virtual ~SupportCalculator() = 0;
    
    virtual bool isAdmissionQuorumPassed(
        const rooset::Policy& policy,
        const unsigned long long totalVoteWeight,
        const unsigned long long support) = 0;
  };

}
