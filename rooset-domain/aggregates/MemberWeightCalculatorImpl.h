#include "MemberWeightCalculator.h"

using namespace std;
namespace rooset {

  class MemberWeightCalculatorImpl : public MemberWeightCalculator
  {
  public:
    unsigned long long calcSupportWeight(
        const uuid& trusteeId,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit) override;

  protected:
    static unsigned long long calcIssueSupportWeight(
        const uuid& trusteeId,
        const unsigned long long weight,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit);

    static unsigned long long calcAreaSupportWeight(
        const uuid& trusteeId,
        const unsigned long long weight,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit);

    static unsigned long long calcUnitSupportWeight(
        const uuid& trusteeId,
        const unsigned long long weight,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit);

    static unsigned long long getMemberStartingWeight(
        const UnitAggregate& unit, const uuid& memberId);
    
    static bool isMemberSupportingInitiativeDirectly(
        const vector<uuid>& supporters, const uuid& trusterId);

    static bool isDelegationBlocked(
        const vector<uuid> blockedDelegations, const uuid& trusterId);
    
    static bool isDelegated(
        const map<uuid, uuid>& delegations, const uuid& trusterId);
  };
}
