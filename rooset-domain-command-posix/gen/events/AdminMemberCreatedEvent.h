
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "ratk/ProjectEvent.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "ratk/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"


using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class AdminMemberCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "ADMIN_MEMBER_CREATED_EVENT";

  public:
    static const SchemaDocument schema;

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
            id(id),
            login(login),
            password(password),
            name(name),
            notifyEmail(notifyEmail)
        {}
  

    
        AdminMemberCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            login(JsonUtils::parseString(d["payload"]["login"])),
            password(JsonUtils::parseString(d["payload"]["password"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
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

      
          payload.AddMember(
              "id",
              JsonUtils::serializeUuid(id, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "login",
              JsonUtils::serializeString(login, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "password",
              JsonUtils::serializeString(password, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          Value notifyEmail_value;
          notifyEmail_value.SetString(notifyEmail.c_str(), notifyEmail.size(), d->GetAllocator());
          payload.AddMember("notifyEmail", notifyEmail_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getMessageType() const override
    {
      return MESSAGE_TYPE;
    }

    uuid getAggregateId() const override
    {
      return id;
    }

  };

};
  