#pragma once
#import <vector>
#import <map>
#import <set>
#import <tuple>
#import "IssueAggregate.h"
#import "UnitAggregate.h"

using namespace std;
namespace rooset {

  class VoteUtils
  {
  public:
    
    static unsigned long long calcSupportWeight(
        const uuid& trusteeId,
        const Initiative& initiative,
        const IssueAggregate& issue,
        const Area& area,
        const UnitAggregate& unit);

    static unsigned long long calcTotalVoteWeight(
        const map<uuid, rooset::MemberPrivilege>& privileges);

    static bool isAdmissionQuorumPassed(
        const rooset::Policy& policy,
        const unsigned long long totalVoteWeight,
        const unsigned long long support);

    static vector<vector<unsigned long long>> calcPairwiseMatrix(
        const vector<SchulzeBallot>& ballots,
        const vector<uuid>& initiativeIds);
    
    static set<int> calcWinners(
        vector<vector<vector<unsigned long long>>>& strogestPathMatrix,
        vector<vector<int>>& winningPairs,
        const vector<vector<unsigned long long>>& pairwiseMatrix);

    static int comparePairwiseBattle(
        const vector<unsigned long long> a,
        const vector<unsigned long long> b);

  private:
    
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
