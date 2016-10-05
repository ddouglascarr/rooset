#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"
#include "../../../framework/ProjectEvent.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class MemberDetailsUpdatedEvent : public ProjectEvent
  {
  public:
    const uuid id;
    const string notifyEmail;

    MemberDetailsUpdatedEvent(uuid id, string notifyEmail) :
      id(id), notifyEmail(notifyEmail)
    {}

    string getEventType() override
    {
      return "MEMBER_DETAILS_UPDATED_EVENT";
    }
  };

}