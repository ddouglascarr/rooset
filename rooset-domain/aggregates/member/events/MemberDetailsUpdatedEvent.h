#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"
#include "../../../framework/ProjectEvent.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class MemberDetailsUpdatedEvent : public ProjectEvent<string>
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
    
    unique_ptr<string> serialize() const override
    {
      return make_unique<string>("{\"foo\":\"bar\"}");
    }
  };

}