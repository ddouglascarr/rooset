#pragma once
#include <iostream>
#include <string>
#include "uuid.h"

using namespace std;

namespace rooset {

  template<typename SerializationFormat>
  class ProjectEvent
  {
  public:
    virtual string getEventType() = 0;
    virtual SerializationFormat toJson() = 0;
  };

}
