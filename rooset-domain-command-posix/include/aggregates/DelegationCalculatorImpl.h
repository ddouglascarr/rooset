#pragma once
#include "DelegationCalculator.h"

using namespace std;
namespace rooset {

  class DelegationCalculatorImpl : public DelegationCalculator
  {
  public:
    map<uuid, uuid> calcInitiativeDelegations(
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit,
        const map<uuid, MemberPrivilege>& memberPrivileges) override;

    map<uuid, uuid> calcIssueDelegations(
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit,
        const map<uuid, MemberPrivilege>& memberPrivileges) override;

    unsigned long long calcMemberWeight(
        const map<uuid, MemberPrivilege>& memberPrivileges,
        const map<uuid, uuid>& delegations,
        const uuid memberid) override;

  protected:
    uuid getUnitDelegation(
      const UnitAggregate& unit,
      const uuid memberId,
      const uuid parentDelegation);

    uuid getAreaDelegation(
      const Area& area,
      const uuid memberId,
      const uuid parentDelegation);

    uuid getIssueDelegation(
      const IssueAggregate& area,
      const uuid memberId,
      const uuid parentDelegation);

    static bool isMemberSupportingInitiativeDirectly(
        const vector<uuid>& supporters, const uuid& trusterId);

    static bool isDelegationBlocked(
        const vector<uuid> blockedDelegations, const uuid& trusterId);

    static bool isMemberVotingOnIssueDirectly(
        const uuid& memberId, const map<uuid, SchulzeBallot>& ballots);
  };
}
