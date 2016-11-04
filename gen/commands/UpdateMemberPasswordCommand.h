
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "framework/uuid.h"
#include "framework/ProjectCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "framework/IdToolsImpl.h"
#include "framework/JsonUtils.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class UpdateMemberPasswordCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "UPDATE_MEMBER_PASSWORD_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const string oldPassword;
        const string newPassword;

    
        UpdateMemberPasswordCommand(
            uuid id,
            uuid requesterId,
            string oldPassword,
            string newPassword) :
            id(id) ,
            requesterId(requesterId) ,
            oldPassword(oldPassword) ,
            newPassword(newPassword) 
        {}
  

    
        UpdateMemberPasswordCommand(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            oldPassword(string(
                d["payload"]["oldPassword"].GetString(),
                d["payload"]["oldPassword"].GetStringLength())),
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

          Value oldPassword_value;
          oldPassword_value.SetString(oldPassword.c_str(), oldPassword.size(), d->GetAllocator());
          payload.AddMember("oldPassword", oldPassword_value, d->GetAllocator());    

          Value newPassword_value;
          newPassword_value.SetString(newPassword.c_str(), newPassword.size(), d->GetAllocator());
          payload.AddMember("newPassword", newPassword_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};
  