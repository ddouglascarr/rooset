#pragma once
#include <string>
#include <memory>
#include "framework/uuid.h"
#include "framework/ProjectEvent.h"
#include "framework/ProjectEvent.h"
#include "../commands/CreateAdminMemberCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "framework/IdToolsImpl.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class AdminMemberCreatedEvent : public ProjectEvent<Document>
  {
  public:
    const uuid id;
    const string login;
    const string password;
    const string name;
    const string notifyEmail;

    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

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

    unique_ptr<Document> serialize() const override
    {
      auto d = make_unique<Document>();
      d->SetObject();

      Value vId; 
      string sId = idTools->serialize(id);
      vId.SetString(sId.c_str(), sId.size(), d->GetAllocator());
      d->AddMember("id", vId, d->GetAllocator());

      Value vLogin;
      vLogin.SetString(login.c_str(), login.size(), d->GetAllocator());
      d->AddMember("login", vLogin, d->GetAllocator());

      Value vPassword;
      vPassword.SetString(password.c_str(), password.size(), d->GetAllocator());
      d->AddMember("password", vPassword, d->GetAllocator());

      Value vName;
      vName.SetString(name.c_str(), name.size(), d->GetAllocator());
      d->AddMember("name", vName, d->GetAllocator());

      Value vNotifyEmail;
      vNotifyEmail.SetString(notifyEmail.c_str(), notifyEmail.size(), d->GetAllocator());
      d->AddMember("notifyEmail", vNotifyEmail, d->GetAllocator());

      return d;
      /*
      StringBuffer buffer;
      Writer<StringBuffer> writer(buffer);
      d.Accept(writer);
      auto ret = string(buffer.GetString(), buffer.GetSize());
      */
    }
  };

}
