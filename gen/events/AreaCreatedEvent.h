
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

  class AreaCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "AREA_CREATED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid areaId;
        const uuid requesterId;
        const string name;
        const string description;
        const string externalReference;

    
        AreaCreatedEvent(
            uuid id,
            uuid areaId,
            uuid requesterId,
            string name,
            string description,
            string externalReference) :
            id(id) ,
            areaId(areaId) ,
            requesterId(requesterId) ,
            name(name) ,
            description(description) ,
            externalReference(externalReference) 
        {}
  

    
        AreaCreatedEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            areaId(idTools->parse(string(
                d["payload"]["areaId"].GetString(),
                d["payload"]["areaId"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            name(string(
                d["payload"]["name"].GetString(),
                d["payload"]["name"].GetStringLength())),
            description(string(
                d["payload"]["description"].GetString(),
                d["payload"]["description"].GetStringLength())),
            externalReference(string(
                d["payload"]["externalReference"].GetString(),
                d["payload"]["externalReference"].GetStringLength()))
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

          Value areaId_value;
          string areaId_str = idTools->serialize(areaId);
          areaId_value.SetString(areaId_str.c_str(), areaId_str.size(), d->GetAllocator());
          payload.AddMember("areaId", areaId_value, d->GetAllocator());    

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

          Value externalReference_value;
          externalReference_value.SetString(externalReference.c_str(), externalReference.size(), d->GetAllocator());
          payload.AddMember("externalReference", externalReference_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  