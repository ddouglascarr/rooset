
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
#include "commands/CreateCompetingInitiativeCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class CompetingInitiativeCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "COMPETING_INITIATIVE_CREATED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;
        const string name;
        const string externalReference;
        const string content;
        const string textSearchData;

    
        CompetingInitiativeCreatedEvent(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            string name,
            string externalReference,
            string content,
            string textSearchData) :
            id(id) ,
            requesterId(requesterId) ,
            initiativeId(initiativeId) ,
            name(name) ,
            externalReference(externalReference) ,
            content(content) ,
            textSearchData(textSearchData) 
        {}
  

    
        CompetingInitiativeCreatedEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            requesterId(idTools->parse(string(
                d["payload"]["requesterId"].GetString(),
                d["payload"]["requesterId"].GetStringLength()))),
            initiativeId(idTools->parse(string(
                d["payload"]["initiativeId"].GetString(),
                d["payload"]["initiativeId"].GetStringLength()))),
            name(string(
                d["payload"]["name"].GetString(),
                d["payload"]["name"].GetStringLength())),
            externalReference(string(
                d["payload"]["externalReference"].GetString(),
                d["payload"]["externalReference"].GetStringLength())),
            content(string(
                d["payload"]["content"].GetString(),
                d["payload"]["content"].GetStringLength())),
            textSearchData(string(
                d["payload"]["textSearchData"].GetString(),
                d["payload"]["textSearchData"].GetStringLength()))
        {}
  

    
    CompetingInitiativeCreatedEvent(const CreateCompetingInitiativeCommand& c):
        id(c.id),
requesterId(c.requesterId),
initiativeId(c.initiativeId),
name(c.name),
externalReference(c.externalReference),
content(c.content),
textSearchData(c.textSearchData)
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

          Value initiativeId_value;
          string initiativeId_str = idTools->serialize(initiativeId);
          initiativeId_value.SetString(initiativeId_str.c_str(), initiativeId_str.size(), d->GetAllocator());
          payload.AddMember("initiativeId", initiativeId_value, d->GetAllocator());    

          Value name_value;
          name_value.SetString(name.c_str(), name.size(), d->GetAllocator());
          payload.AddMember("name", name_value, d->GetAllocator());    

          Value externalReference_value;
          externalReference_value.SetString(externalReference.c_str(), externalReference.size(), d->GetAllocator());
          payload.AddMember("externalReference", externalReference_value, d->GetAllocator());    

          Value content_value;
          content_value.SetString(content.c_str(), content.size(), d->GetAllocator());
          payload.AddMember("content", content_value, d->GetAllocator());    

          Value textSearchData_value;
          textSearchData_value.SetString(textSearchData.c_str(), textSearchData.size(), d->GetAllocator());
          payload.AddMember("textSearchData", textSearchData_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  