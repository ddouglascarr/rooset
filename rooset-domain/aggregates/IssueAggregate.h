#pragma once
#include <string>
#include <map>
#include "framework/uuid.h"
#include "events/NewInitiativeCreatedEvent.h"
#include "events/CompetingInitiativeCreatedEvent.h"
#include "events/IssueDelegationSetEvent.h"
#include "events/IssueDelegationUnsetEvent.h"

using namespace std;

namespace rooset {

  struct Initiative {
    string name;
    uuid initiatorId;
  };
  
  class IssueAggregate
  {
  private:
    const uuid id;
    const uuid unitId;
    const uuid areaId;
    map<uuid, Initiative> initiatives;
    map<uuid, uuid> delegations;

  public:
    IssueAggregate(const NewInitiativeCreatedEvent& e);
    void handleEvent(const CompetingInitiativeCreatedEvent& e);
    void handleEvent(const IssueDelegationSetEvent& e);
    void handleEvent(const IssueDelegationUnsetEvent& e);


    inline uuid getId() const { return id; }
    inline uuid getUnitId() const { return unitId; }
    inline uuid getAreaId() const { return areaId; }
    inline auto getInitiatives() const { return initiatives; }
    inline auto getDelegations() const { return delegations; }
  };
}