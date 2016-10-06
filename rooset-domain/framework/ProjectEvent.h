#pragma once
#include <iostream>
#include <memory>
#include <string>
#include "uuid.h"

using namespace std;

namespace rooset {

  template<typename SerializationFormat>
  class ProjectEvent
  {
  public:
    virtual string getEventType() = 0;
    virtual unique_ptr<SerializationFormat> serialize() const = 0;
  };

}
