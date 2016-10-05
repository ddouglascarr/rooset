#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class CreateUnitCommand
  {
  public:
    const uuid id;
    const uuid parentId;
    const string name;
    const string description;

    CreateUnitCommand(
      uuid id, uuid parentId, string name, string description) :
        id(id.is_nil() ? throw string("Must not be nil") : id),
        parentId(parentId), name(name), description(description)
    {}
  };

}

