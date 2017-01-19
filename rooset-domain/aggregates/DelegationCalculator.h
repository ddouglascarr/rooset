#pragma once
#include <map>
#include "UnitAggregate.h"
#include "IssueAggregate.h"

using namespace std;
namespace rooset {

  class DelegationCalculator
  {
  public:
    virtual ~DelegationCalculator() {};

    virtual map<uuid, uuid> calcInitiativeDelegations(
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit,
        const map<uuid, MemberPrivilege>& memberPrivileges) =0;

    virtual map<uuid, uuid> calcIssueDelegations(
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit,
        const map<uuid, MemberPrivilege>& memberPrivileges) =0;

    virtual unsigned long long calcMemberWeight(
        const map<uuid, MemberPrivilege>& memberPrivileges,
        const map<uuid, uuid>& delegations,
        const uuid memberid) =0;

  };

}
