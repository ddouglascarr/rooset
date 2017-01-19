
#pragma once
#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "framework/uuid.h"
#include "framework/ProjectCommand.h"
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

  class UnblockDelegationForAreaCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "UNBLOCK_DELEGATION_FOR_AREA_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid areaId;
        const uuid requesterId;

    
        UnblockDelegationForAreaCommand(
            uuid id,
            uuid areaId,
            uuid requesterId) :
            id(id),
            areaId(areaId),
            requesterId(requesterId)
        {}
  

    
        UnblockDelegationForAreaCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"]))
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

          Value areaId_value;
          string areaId_str = idTools->serialize(areaId);
          areaId_value.SetString(areaId_str.c_str(), areaId_str.size(), d->GetAllocator());
          payload.AddMember("areaId", areaId_value, d->GetAllocator());    

          Value requesterId_value;
          string requesterId_str = idTools->serialize(requesterId);
          requesterId_value.SetString(requesterId_str.c_str(), requesterId_str.size(), d->GetAllocator());
          payload.AddMember("requesterId", requesterId_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};
  