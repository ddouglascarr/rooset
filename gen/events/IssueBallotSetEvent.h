
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
#include "commands/SetIssueBallotCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class IssueBallotSetEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "ISSUE_BALLOT_SET_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const rooset::SchulzeBallot ballot;

    
        IssueBallotSetEvent(
            uuid id,
            uuid requesterId,
            rooset::SchulzeBallot ballot) :
            id(id) ,
            requesterId(requesterId) ,
            ballot(ballot)
        {}
  

    
        IssueBallotSetEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            ballot(rooset::SchulzeBallot::parse(d["payload"]["ballot"]))
        {}
  

    
    IssueBallotSetEvent(const SetIssueBallotCommand& c):
        id(c.id),
requesterId(c.requesterId),
ballot(c.ballot)
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

          Value requesterId_value;
          string requesterId_str = idTools->serialize(requesterId);
          requesterId_value.SetString(requesterId_str.c_str(), requesterId_str.size(), d->GetAllocator());
          payload.AddMember("requesterId", requesterId_value, d->GetAllocator());    

          auto ballot_value = rooset::SchulzeBallot::serialize(ballot, d->GetAllocator());
          payload.AddMember("ballot", ballot_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  