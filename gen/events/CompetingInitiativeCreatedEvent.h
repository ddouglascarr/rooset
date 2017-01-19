
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
#include "aggregates/SchulzeBallot.h"
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
        const boost::posix_time::ptime created;

    
        CompetingInitiativeCreatedEvent(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            string name,
            string externalReference,
            string content,
            string textSearchData,
            boost::posix_time::ptime created) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId),
            name(name),
            externalReference(externalReference),
            content(content),
            textSearchData(textSearchData),
            created(created)
        {}
  

    
        CompetingInitiativeCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            externalReference(JsonUtils::parseString(d["payload"]["externalReference"])),
            content(JsonUtils::parseString(d["payload"]["content"])),
            textSearchData(JsonUtils::parseString(d["payload"]["textSearchData"])),
            created(boost::posix_time::from_time_t(
                time_t(d["payload"]["created"].GetDouble())))
        {}
  

    
    CompetingInitiativeCreatedEvent(const CreateCompetingInitiativeCommand& c):
        id(c.id),
requesterId(c.requesterId),
initiativeId(c.initiativeId),
name(c.name),
externalReference(c.externalReference),
content(c.content),
textSearchData(c.textSearchData),
created(c.created)
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

          JsonUtils::serializeString(name, d->GetAllocator());

          JsonUtils::serializeString(externalReference, d->GetAllocator());

          JsonUtils::serializeString(content, d->GetAllocator());

          JsonUtils::serializeString(textSearchData, d->GetAllocator());

          Value created_value;
          created_value.SetDouble(boost::posix_time::to_time_t(created));
          payload.AddMember("created", created_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  