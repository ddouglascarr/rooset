
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
#include "commands/CreateNewInitiativeCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class NewInitiativeCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "NEW_INITIATIVE_CREATED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;
        const uuid unitId;
        const uuid areaId;
        const uuid policyId;
        const string name;
        const bool polling;
        const string externalReference;
        const string content;
        const string textSearchData;
        const boost::posix_time::ptime created;

    
        NewInitiativeCreatedEvent(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            uuid unitId,
            uuid areaId,
            uuid policyId,
            string name,
            bool polling,
            string externalReference,
            string content,
            string textSearchData,
            boost::posix_time::ptime created) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId),
            unitId(unitId),
            areaId(areaId),
            policyId(policyId),
            name(name),
            polling(polling),
            externalReference(externalReference),
            content(content),
            textSearchData(textSearchData),
            created(created)
        {}
  

    
        NewInitiativeCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"])),
            unitId(JsonUtils::parseUuid(d["payload"]["unitId"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            policyId(JsonUtils::parseUuid(d["payload"]["policyId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            polling(d["payload"]["polling"].GetBool()),
            externalReference(JsonUtils::parseString(d["payload"]["externalReference"])),
            content(JsonUtils::parseString(d["payload"]["content"])),
            textSearchData(JsonUtils::parseString(d["payload"]["textSearchData"])),
            created(boost::posix_time::from_time_t(
                time_t(d["payload"]["created"].GetDouble())))
        {}
  

    
    NewInitiativeCreatedEvent(const CreateNewInitiativeCommand& c):
        id(c.id),
requesterId(c.requesterId),
initiativeId(c.initiativeId),
unitId(c.unitId),
areaId(c.areaId),
policyId(c.policyId),
name(c.name),
polling(c.polling),
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

      
          payload.AddMember(
              "id",
              JsonUtils::serializeUuid(id, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "requesterId",
              JsonUtils::serializeUuid(requesterId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "initiativeId",
              JsonUtils::serializeUuid(initiativeId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "unitId",
              JsonUtils::serializeUuid(unitId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "areaId",
              JsonUtils::serializeUuid(areaId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "policyId",
              JsonUtils::serializeUuid(policyId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          Value polling_value;
          polling_value.SetBool(polling);
          payload.AddMember("polling", polling_value, d->GetAllocator());

          payload.AddMember(
              "externalReference",
              JsonUtils::serializeString(externalReference, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "content",
              JsonUtils::serializeString(content, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "textSearchData",
              JsonUtils::serializeString(textSearchData, d->GetAllocator()),
              d->GetAllocator());

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
  