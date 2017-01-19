
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
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class UpdateMemberDetailsCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "UPDATE_MEMBER_DETAILS_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const string notifyEmail;

    
        UpdateMemberDetailsCommand(
            uuid id,
            string notifyEmail) :
            id(id),
            notifyEmail(notifyEmail)
        {}
  

    
        UpdateMemberDetailsCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
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

          Value notifyEmail_value;
          notifyEmail_value.SetString(notifyEmail.c_str(), notifyEmail.size(), d->GetAllocator());
          payload.AddMember("notifyEmail", notifyEmail_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};
  