#pragma once
#include <string>
#include <memory>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "enums/InitiativeContentType.h"
#include "events/UnitCreatedEvent.h"
#include "events/AreaCreatedEvent.h"
#include "events/PrivilegeGrantedEvent.h"
#include "events/UnitDelegationSetEvent.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "events/AreaDelegationSetEvent.h"
#include "events/AreaDelegationUnsetEvent.h"
#include "events/PolicyAddedEvent.h"
#include "events/DelegationBlockedForAreaEvent.h"
#include "events/DelegationUnblockedForAreaEvent.h"  
#include "events/ConcernCreatedEvent.h"
#include "events/ConcernDeactivatedEvent.h"
#include "events/AreaConcernAddedEvent.h"
#include "events/AreaConcernRemovedEvent.h"

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
    vector<uuid> concerns;
  };

  struct Policy {
    string name;
    bool active;
    unsigned int issueQuorumNum;
    unsigned int issueQuorumDen;
    unsigned int initiativeQuorumNum;
    unsigned int initiativeQuorumDen;
  };

  struct Concern {
    bool active;
    InitiativeContentType initiativeContentType;
    string config;
  };
  
  class UnitAggregate
  {
  private:
    map<uuid, Area> areas;
    map<uuid, MemberPrivilege> privileges;
    map<uuid, uuid> delegations;
    map<uuid, Policy> policies;
    map<uuid, Concern> concerns;

  public:
    UnitAggregate(const UnitCreatedEvent& e);
    void handleEvent(const AreaCreatedEvent& e);
    void handleEvent(const PrivilegeGrantedEvent& e);
    void handleEvent(const PolicyAddedEvent& e);
    void handleEvent(const UnitDelegationSetEvent& e);
    void handleEvent(const UnitDelegationUnsetEvent& e);
    void handleEvent(const AreaDelegationSetEvent& e);
    void handleEvent(const AreaDelegationUnsetEvent& e);
    void handleEvent(const DelegationBlockedForAreaEvent& e);
    void handleEvent(const DelegationUnblockedForAreaEvent& e);
    void handleEvent(const ConcernCreatedEvent& e);
    void handleEvent(const ConcernDeactivatedEvent& e);
    void handleEvent(const AreaConcernAddedEvent& e);
    void handleEvent(const AreaConcernRemovedEvent& e);


    inline auto getPrivileges() const { return privileges; }
    inline auto getDelegations() const { return delegations; }
    inline auto getAreas() const { return areas; }
    inline auto getPolicies() const { return policies; }
    inline auto getConcerns() const { return concerns; }
  };
}
