
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
#include "framework/IdUtils.h"
#include "framework/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;


namespace rooset {

  class BlockDelegationForIssueCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "BLOCK_DELEGATION_FOR_ISSUE_COMMAND";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;

    
        BlockDelegationForIssueCommand(
            uuid id,
            uuid requesterId) :
            id(id),
            requesterId(requesterId)
        {}
  

    
        BlockDelegationForIssueCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
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

      
          payload.AddMember(
              "id",
              JsonUtils::serializeUuid(id, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "requesterId",
              JsonUtils::serializeUuid(requesterId, d->GetAllocator()),
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