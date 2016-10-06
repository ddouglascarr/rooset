#pragma once
#include <iostream>
#include <string>
#include "uuid.h"

using namespace std;

namespace rooset {

  class ProjectEvent
  {
  public:
    virtual string getEventType() = 0;
    virtual string toJson() = 0;
  };

}
