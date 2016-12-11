
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "framework/uuid.h"
#include "framework/ProjectEvent.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "framework/IdToolsImpl.h"
#include "framework/JsonUtils.h"
#include "enums/EnumUtils.h"


using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class AdminMemberCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "ADMIN_MEMBER_CREATED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const string login;
        const string password;
        const string name;
        const string notifyEmail;

    
        AdminMemberCreatedEvent(
            uuid id,
            string login,
            string password,
            string name,
            string notifyEmail) :
            id(id) ,
            login(login) ,
            password(password) ,
            name(name) ,
            notifyEmail(notifyEmail) 
        {}
  

    
        AdminMemberCreatedEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            login(string(
                d["payload"]["login"].GetString(),
                d["payload"]["login"].GetStringLength())),
            password(string(
                d["payload"]["password"].GetString(),
                d["payload"]["password"].GetStringLength())),
            name(string(
                d["payload"]["name"].GetString(),
                d["payload"]["name"].GetStringLength())),
            notifyEmail(string(
                d["payload"]["notifyEmail"].GetString(),
                d["payload"]["notifyEmail"].GetStringLength()))
        {}
  

    

    unique_ptr<Document> serialize() const override
    {
      auto d = make_unique<Document>();
      d->SetObject();

      Value payload;
      payload.SetObject();

      Value messageTypeValue;
      messageTypeValue.SetString(MESSAGE_TYPE.c_str(),
          MESSAGE_TYPE.size(), d->GetAllocator());
      d->AddMember("type", messageTypeValue, d->GetAllocator());

      
          Value id_value;
          string id_str = idTools->serialize(id);
          id_value.SetString(id_str.c_str(), id_str.size(), d->GetAllocator());
          payload.AddMember("id", id_value, d->GetAllocator());    

          Value login_value;
          login_value.SetString(login.c_str(), login.size(), d->GetAllocator());
          payload.AddMember("login", login_value, d->GetAllocator());    

          Value password_value;
          password_value.SetString(password.c_str(), password.size(), d->GetAllocator());
          payload.AddMember("password", password_value, d->GetAllocator());    

          Value name_value;
          name_value.SetString(name.c_str(), name.size(), d->GetAllocator());
          payload.AddMember("name", name_value, d->GetAllocator());    

          Value notifyEmail_value;
          notifyEmail_value.SetString(notifyEmail.c_str(), notifyEmail.size(), d->GetAllocator());
          payload.AddMember("notifyEmail", notifyEmail_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  