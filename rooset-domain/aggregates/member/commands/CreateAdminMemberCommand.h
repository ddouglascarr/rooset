#pragma once
#include <string>
#include "../../../framework/uuid.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class CreateAdminMemberCommand
  {
  public:
    const uuid id;
    const string login;
    const string password;
    const string name;
    const string notifyEmail;

    CreateAdminMemberCommand(
      uuid id, string login, string password,
      string name, string notifyEmail) :
      id(id.is_nil() ? throw string("Must not be nil") : id),
      login(login), password(password), name(name),
      notifyEmail(notifyEmail)
    {}
  };

}


