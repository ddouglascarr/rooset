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
    virtual ~ProjectEvent() {};
    virtual string getMessageType() const = 0;
    virtual uuid getAggregateId() const = 0;
    virtual unique_ptr<SerializationFormat> serialize() const = 0;
  };

}