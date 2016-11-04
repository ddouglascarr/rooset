#pragma once
#include <string>
#include <memory>
#include <map>
#include "framework/uuid.h"
#include "events/UnitCreatedEvent.h"
#include "events/AreaCreatedEvent.h"
#include "events/PrivilegeGrantedEvent.h"
#include "events/UnitDelegationSetEvent.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "events/AreaDelegationSetEvent.h"
#include "events/AreaDelegationUnsetEvent.h"

using namespace std;

namespace rooset {

  struct MemberPrivilege
  {
    bool pollingRight;
    bool votingRight;
    bool initiativeRight;
    bool managementRight;
  };

  struct Area {
    string name;
    map<uuid, uuid> delegations;
  };

  class UnitAggregate
  {
  private:
    map<uuid, Area> areas;
    map<uuid, MemberPrivilege> privileges;
    map<uuid, uuid> delegations;

  public:
    UnitAggregate(const UnitCreatedEvent& e);
    void handleEvent(const AreaCreatedEvent& e);
    void handleEvent(const PrivilegeGrantedEvent& e);
    void handleEvent(const UnitDelegationSetEvent& e);
    void handleEvent(const UnitDelegationUnsetEvent& e);
    void handleEvent(const AreaDelegationSetEvent& e);
    void handleEvent(const AreaDelegationUnsetEvent& e);

    inline auto getPrivileges() const { return privileges; }
    inline auto getDelegations() const { return delegations; }
    inline auto getAreas() const { return areas; }
  };
}