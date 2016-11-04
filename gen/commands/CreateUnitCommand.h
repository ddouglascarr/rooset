
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

  class CreateUnitCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "CREATE_UNIT_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const string name;
        const string description;

    
        CreateUnitCommand(
            uuid id,
            uuid requesterId,
            string name,
            string description) :
            id(id) ,
            requesterId(requesterId) ,
            name(name) ,
            description(description) 
        {}
  

    
        CreateUnitCommand(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            name(string(
                d["payload"]["name"].GetString(),
                d["payload"]["name"].GetStringLength())),
            description(string(
                d["payload"]["description"].GetString(),
                d["payload"]["description"].GetStringLength()))
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

          Value name_value;
          name_value.SetString(name.c_str(), name.size(), d->GetAllocator());
          payload.AddMember("name", name_value, d->GetAllocator());    

          Value description_value;
          description_value.SetString(description.c_str(), description.size(), d->GetAllocator());
          payload.AddMember("description", description_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};
  