#pragma once
#include <iostream>
#include <string>
#include "../../../framework/uuid.h"

using namespace std;
using namespace rooset;

namespace rooset {

  class UpdateMemberPasswordCommand
  {
  public:
    const uuid id;
    const uuid requesterId;
    const string oldPassword;
    const string newPassword;

    UpdateMemberPasswordCommand(
      uuid id, uuid requesterId, string oldPassword, string newPassword) :
      id(id.is_nil() ? throw string("Must not be nil") : id),
      requesterId(requesterId.is_nil() ? throw string("Must not be nil") : requesterId),
      oldPassword(oldPassword), newPassword(newPassword)
    {}
  };

}
