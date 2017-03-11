
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
#include "commands/CreateConcernCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class ConcernCreatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "CONCERN_CREATED_EVENT";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid concernId;
        const uuid requesterId;
        const string name;
        const string description;
        const rooset::InitiativeContentType initiativeContentType;
        const string config;

    
        ConcernCreatedEvent(
            uuid id,
            uuid concernId,
            uuid requesterId,
            string name,
            string description,
            rooset::InitiativeContentType initiativeContentType,
            string config) :
            id(id),
            concernId(concernId),
            requesterId(requesterId),
            name(name),
            description(description),
            initiativeContentType(initiativeContentType),
            config(config)
        {}
  

    
        ConcernCreatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            concernId(JsonUtils::parseUuid(d["payload"]["concernId"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            description(JsonUtils::parseString(d["payload"]["description"])),
            initiativeContentType(EnumUtils::parseInitiativeContentType(
                JsonUtils::parseString(d["payload"]["initiativeContentType"]))),
            config(JsonUtils::parseString(d["payload"]["config"]))
        {}
  

    
    ConcernCreatedEvent(const CreateConcernCommand& c):
        id(c.id),
concernId(c.concernId),
requesterId(c.requesterId),
name(c.name),
description(c.description),
initiativeContentType(c.initiativeContentType),
config(c.config)
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
              "concernId",
              JsonUtils::serializeUuid(concernId, d->GetAllocator()),
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
              "initiativeContentType",
              JsonUtils::serializeString(
                  EnumUtils::serializeInitiativeContentType(initiativeContentType),
                  d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "config",
              JsonUtils::serializeString(config, d->GetAllocator()),
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
  