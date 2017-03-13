
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
#include "commands/DeactivateConcernCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class ConcernDeactivatedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "CONCERN_DEACTIVATED_EVENT";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid concernId;

    
        ConcernDeactivatedEvent(
            uuid id,
            uuid requesterId,
            uuid concernId) :
            id(id),
            requesterId(requesterId),
            concernId(concernId)
        {}
  

    
        ConcernDeactivatedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            concernId(JsonUtils::parseUuid(d["payload"]["concernId"]))
        {}
  

    
    ConcernDeactivatedEvent(const DeactivateConcernCommand& c):
        id(c.id),
requesterId(c.requesterId),
concernId(c.concernId)
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
              "concernId",
              JsonUtils::serializeUuid(concernId, d->GetAllocator()),
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
  