#pragma once
#include <string>
#include "framework/uuid.h"
#include "events/MemberCreatedEvent.h"
#include "events/AdminMemberCreatedEvent.h"
#include "events/MemberPasswordUpdatedEvent.h"
#include "events/MemberDetailsUpdatedEvent.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class MemberAggregate
  {
  private:
    const uuid id;
    const bool isAdmin;
    string login;
    string password;
    string notifyEmail;

  public:
    MemberAggregate(const AdminMemberCreatedEvent& e) :
        id(e.id), isAdmin(true), login(e.login), password(e.password),
        notifyEmail(e.notifyEmail)
    {}

    MemberAggregate(const MemberCreatedEvent& e) :
        id(e.id), isAdmin(false), login(e.login), password(e.password),
        notifyEmail(e.notifyEmail)
    {}

    void handleEvent(const MemberPasswordUpdatedEvent& e)
    {
      password = e.newPassword;
    }

    void handleEvent(const MemberDetailsUpdatedEvent& e)
    {
      notifyEmail = e.notifyEmail;
    }

    uuid getId() const { return id; }
    bool getIsAdmin() const { return isAdmin; }
    string getLogin() const { return login; }
    string getPassword() const { return password; }
    string getNotifyEmail() const { return notifyEmail; }
  };

}