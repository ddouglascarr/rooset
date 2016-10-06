#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"
#include "../../../framework/ProjectEvent.h"
#include "../commands/UpdateMemberPasswordCommand.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class MemberPasswordUpdatedEvent : public ProjectEvent
  {
  public:
    const uuid id;
    const uuid requesterId;
    const string newPassword;

    MemberPasswordUpdatedEvent(UpdateMemberPasswordCommand c) :
      id(c.id.is_nil() ? throw string("Must not be nil") : c.id),
      requesterId(c.requesterId.is_nil() ? throw string("Must not be nil") : requesterId),
      newPassword(c.newPassword)
    {}

    string getEventType() override
    {
      return "MEMBER_PASSWORD_UPDATED_EVENT";
    }

    string toJson() override
    {
      return "{\"foo\":\"bar\"}";
    }
  };

}