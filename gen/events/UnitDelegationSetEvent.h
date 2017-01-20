
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


using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class UnitDelegationSetEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "UNIT_DELEGATION_SET_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid trusterId;
        const uuid trusteeId;

    
        UnitDelegationSetEvent(
            uuid id,
            uuid trusterId,
            uuid trusteeId) :
            id(id),
            trusterId(trusterId),
            trusteeId(trusteeId)
        {}
  

    
        UnitDelegationSetEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            trusterId(JsonUtils::parseUuid(d["payload"]["trusterId"])),
            trusteeId(JsonUtils::parseUuid(d["payload"]["trusteeId"]))
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
              "trusterId",
              JsonUtils::serializeUuid(trusterId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "trusteeId",
              JsonUtils::serializeUuid(trusteeId, d->GetAllocator()),
              d->GetAllocator());

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  