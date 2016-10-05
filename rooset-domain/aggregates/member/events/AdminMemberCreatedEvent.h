#pragma once
#include <string>
#include "../../../framework/uuid.h"
#include "../../../framework/ProjectEvent.h"
#include "../../../framework/ProjectEvent.h"
#include "../commands/CreateAdminMemberCommand.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class AdminMemberCreatedEvent : public ProjectEvent
  {
  public:
    const uuid id;
    const string login;
    const string password;
    const string name;
    const string notifyEmail;

    AdminMemberCreatedEvent(
      CreateAdminMemberCommand c) :
      id(c.id.is_nil() ? throw string("Must not be nil") : c.id),
      login(c.login), password(c.password), name(c.name),
      notifyEmail(c.notifyEmail)
    {}

    string getEventType() override
    {
      return "ADMIN_MEMBER_CREATED_EVENT";
    }
  };

}