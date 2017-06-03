
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "ratk/uuid.h"
#include "ratk/ProjectCommand.h"
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/schema.h"
#include "ratk/IdUtils.h"
#include "ratk/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class SetAreaDelegationCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "SET_AREA_DELEGATION_COMMAND";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid areaId;
        const uuid requesterId;
        const uuid trusteeId;

    
        SetAreaDelegationCommand(
            uuid id,
            uuid areaId,
            uuid requesterId,
            uuid trusteeId) :
            id(id),
            areaId(areaId),
            requesterId(requesterId),
            trusteeId(trusteeId)
        {}
  

    
        SetAreaDelegationCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
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
              "areaId",
              JsonUtils::serializeUuid(areaId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "requesterId",
              JsonUtils::serializeUuid(requesterId, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "trusteeId",
              JsonUtils::serializeUuid(trusteeId, d->GetAllocator()),
              d->GetAllocator());

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};