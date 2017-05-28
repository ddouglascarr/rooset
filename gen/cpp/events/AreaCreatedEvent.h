
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

  class AreaCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "AREA_CREATED_EVENT";

  public:
    static const SchemaDocument schema;

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
            id(id),
            areaId(areaId),
            requesterId(requesterId),
            name(name),
            description(description),
            externalReference(externalReference)
        {}
  

    
        AreaCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            description(JsonUtils::parseString(d["payload"]["description"])),
            externalReference(JsonUtils::parseString(d["payload"]["externalReference"]))
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
              "areaId",
              JsonUtils::serializeUuid(areaId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "requesterId",
              JsonUtils::serializeUuid(requesterId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "description",
              JsonUtils::serializeString(description, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "externalReference",
              JsonUtils::serializeString(externalReference, d->GetAllocator()),
              d->GetAllocator());

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
  