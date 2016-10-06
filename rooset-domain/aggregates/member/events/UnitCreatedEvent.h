#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"
#include "../../../framework/ProjectEvent.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class UnitCreatedEvent : public ProjectEvent
  {
  public:
    const uuid id;
    const uuid parentId;
    const string name;
    const string description;

    UnitCreatedEvent(
      uuid id, uuid parentId, string name, string description) :
      id(id.is_nil() ? throw string("Must not be nil") : id),
      parentId(parentId), name(name), description(description)
    {}

    string getEventType() override
    {
      return "UNIT_CREATED_EVENT";
    }

    string toJson() override
    {
      return "{\"foo\":\"bar\"}";
    }
  };

}