
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

  class MemberPasswordUpdatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "MEMBER_PASSWORD_UPDATED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const string newPassword;

    
        MemberPasswordUpdatedEvent(
            uuid id,
            uuid requesterId,
            string newPassword) :
            id(id) ,
            requesterId(requesterId) ,
            newPassword(newPassword) 
        {}
  

    
        MemberPasswordUpdatedEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            newPassword(string(
                d["payload"]["newPassword"].GetString(),
                d["payload"]["newPassword"].GetStringLength()))
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

          Value requesterId_value;
          string requesterId_str = idTools->serialize(requesterId);
          requesterId_value.SetString(requesterId_str.c_str(), requesterId_str.size(), d->GetAllocator());
          payload.AddMember("requesterId", requesterId_value, d->GetAllocator());    

          Value newPassword_value;
          newPassword_value.SetString(newPassword.c_str(), newPassword.size(), d->GetAllocator());
          payload.AddMember("newPassword", newPassword_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  