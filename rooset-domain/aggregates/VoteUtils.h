#pragma once
#import <vector>
#import <map>
#import "IssueAggregate.h"
#import "UnitAggregate.h"

using namespace std;
namespace rooset {

  class VoteUtils
  {
  public:
    
    static int calcSupportWeight(
      const uuid& trusteeId,
      const Initiative& initiative,
      const IssueAggregate& issue,
      const Area& area,
      const UnitAggregate& unit);

    static int calcTotalVoteWeight(const map<uuid, rooset::MemberPrivilege>& privileges);

    static bool isAdmissionQuorumPassed(
        const rooset::Policy& policy, const int totalVoteWeight, const int support);

  private:
    
    static int calcIssueSupportWeight(
      const uuid& trusteeId,
      const int weight,
      const Initiative& initiative,
      const IssueAggregate& issue,
      const Area& area,
      const UnitAggregate& unit);

    static int calcAreaSupportWeight(
      const uuid& trusteeId,
      const int weight,
      const Initiative& initiative,
      const IssueAggregate& issue,
      const Area& area,
      const UnitAggregate& unit);

    static int calcUnitSupportWeight(
      const uuid& trusteeId,
      const int weight,
      const Initiative& initiative,
      const IssueAggregate& issue,
      const Area& area,
      const UnitAggregate& unit);

    static int getMemberStartingWeight(const UnitAggregate& unit, const uuid& memberId);
    
    static bool isMemberSupportingInitiativeDirectly(
        const vector<uuid>& supporters, const uuid& trusterId);

    static bool isDelegationBlocked(
        const vector<uuid> blockedDelegations, const uuid& trusterId);
    
    static bool isDelegated(
        const map<uuid, uuid>& delegations, const uuid& trusterId);

  };

}