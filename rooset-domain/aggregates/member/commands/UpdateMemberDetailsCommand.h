#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"

using namespace std;
using namespace rooset;

namespace rooset {
  
  class UpdateMemberDetailsCommand
  {
  public:
    const uuid id;
    const string notifyEmail;

    UpdateMemberDetailsCommand(uuid id, string notifyEmail) :
      id(id), notifyEmail(notifyEmail)
    {}
  };
}