#pragma once
#include <string>
#include <memory>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "events/UnitCreatedEvent.h"
#include "events/AreaCreatedEvent.h"
#include "events/PrivilegeGrantedEvent.h"
#include "events/UnitDelegationSetEvent.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "events/AreaDelegationSetEvent.h"
#include "events/AreaDelegationUnsetEvent.h"
#include "events/UnitPolicySetEvent.h"
#include "events/DelegationBlockedForAreaEvent.h"
#include "events/DelegationUnblockedForAreaEvent.h"

using namespace std;

namespace rooset {

  struct MemberPrivilege
  {
    bool pollingRight;
    bool votingRight;
    bool initiativeRight;
    bool managementRight;
    unsigned long long weight;
  };

  struct Area {
    string name;
    map<uuid, uuid> delegations;
    vector<uuid> blockedDelegations;
  };

  struct Policy {
    string name;
    bool active;
    unsigned int issueQuorumNum;
    unsigned int issueQuorumDen;
  };

  class UnitAggregate
  {
  private:
    map<uuid, Area> areas;
    map<uuid, MemberPrivilege> privileges;
    map<uuid, uuid> delegations;
    map<uuid, Policy> policies;

  public:
    UnitAggregate(const UnitCreatedEvent& e);
    void handleEvent(const AreaCreatedEvent& e);
    void handleEvent(const PrivilegeGrantedEvent& e);
    void handleEvent(const UnitDelegationSetEvent& e);
    void handleEvent(const UnitDelegationUnsetEvent& e);
    void handleEvent(const AreaDelegationSetEvent& e);
    void handleEvent(const AreaDelegationUnsetEvent& e);
    void handleEvent(const DelegationBlockedForAreaEvent& e);
    void handleEvent(const DelegationUnblockedForAreaEvent& e);

    void handleEvent(const UnitPolicySetEvent& e);

    inline auto getPrivileges() const { return privileges; }
    inline auto getDelegations() const { return delegations; }
    inline auto getAreas() const { return areas; }
    inline auto getPolicies() const { return policies; }
  };
}