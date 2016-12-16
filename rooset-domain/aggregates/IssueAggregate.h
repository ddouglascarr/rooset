#pragma once
#include <string>
#include <map>
#include "framework/uuid.h"
#include "events/NewInitiativeCreatedEvent.h"

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

  public:
    IssueAggregate(const NewInitiativeCreatedEvent& e);

    inline uuid getId() const { return id; }
    inline uuid getUnitId() const { return unitId; }
    inline uuid getAreaId() const { return areaId; }
  };
}