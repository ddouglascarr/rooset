
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
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;
        const uuid unitId;
        const uuid areaId;
        const uuid policyId;
        const uuid concernId;
        const string name;
        const string content;
        const unsigned int created;

    
        NewInitiativeCreatedEvent(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            uuid unitId,
            uuid areaId,
            uuid policyId,
            uuid concernId,
            string name,
            string content,
            unsigned int created) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId),
            unitId(unitId),
            areaId(areaId),
            policyId(policyId),
            concernId(concernId),
            name(name),
            content(content),
            created(created)
        {}
  

    
        NewInitiativeCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"])),
            unitId(JsonUtils::parseUuid(d["payload"]["unitId"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            policyId(JsonUtils::parseUuid(d["payload"]["policyId"])),
            concernId(JsonUtils::parseUuid(d["payload"]["concernId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            content(JsonUtils::parseString(d["payload"]["content"])),
            created(d["payload"]["created"].GetUint())
        {}
  

    
    NewInitiativeCreatedEvent(const CreateNewInitiativeCommand& c):
        id(c.id),
requesterId(c.requesterId),
initiativeId(c.initiativeId),
unitId(c.unitId),
areaId(c.areaId),
policyId(c.policyId),
concernId(c.concernId),
name(c.name),
content(c.content),
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
              "concernId",
              JsonUtils::serializeUuid(concernId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "content",
              JsonUtils::serializeString(content, d->GetAllocator()),
              d->GetAllocator());

          Value created_value;
          created_value.SetUint(created);
          payload.AddMember("created", created_value, d->GetAllocator());    

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
  