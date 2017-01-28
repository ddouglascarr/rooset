
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
#include "commands/GiveInitiativeSupportCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class InitiativeSupportGivenEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "INITIATIVE_SUPPORT_GIVEN_EVENT";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;

    
        InitiativeSupportGivenEvent(
            uuid id,
            uuid requesterId,
            uuid initiativeId) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId)
        {}
  

    
        InitiativeSupportGivenEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"]))
        {}
  

    
    InitiativeSupportGivenEvent(const GiveInitiativeSupportCommand& c):
        id(c.id),
requesterId(c.requesterId),
initiativeId(c.initiativeId)
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
  