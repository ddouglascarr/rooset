#include "UnitAggregate.h"
#include "IssueAggregate.h"

using namespace std;
namespace rooset {

  class MemberWeightCalculator
  {
  public:
    virtual ~MemberWeightCalculator() {};
    virtual unsigned long long calcSupportWeight(
        const uuid& trusteeId,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit) = 0;
  };

}
