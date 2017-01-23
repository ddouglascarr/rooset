
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
#include "framework/JsonUtils.h"
#include "enums/EnumUtils.h"
#include "aggregates/SchulzeBallot.h"
#include "commands/GrantPrivilegeCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class PrivilegeGrantedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "PRIVILEGE_GRANTED_EVENT";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid memberId;
        const bool pollingRight;
        const bool votingRight;
        const bool initiativeRight;
        const bool managementRight;
        const unsigned int weight;

    
        PrivilegeGrantedEvent(
            uuid id,
            uuid requesterId,
            uuid memberId,
            bool pollingRight,
            bool votingRight,
            bool initiativeRight,
            bool managementRight,
            unsigned int weight) :
            id(id),
            requesterId(requesterId),
            memberId(memberId),
            pollingRight(pollingRight),
            votingRight(votingRight),
            initiativeRight(initiativeRight),
            managementRight(managementRight),
            weight(weight)
        {}
  

    
        PrivilegeGrantedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            memberId(JsonUtils::parseUuid(d["payload"]["memberId"])),
            pollingRight(d["payload"]["pollingRight"].GetBool()),
            votingRight(d["payload"]["votingRight"].GetBool()),
            initiativeRight(d["payload"]["initiativeRight"].GetBool()),
            managementRight(d["payload"]["managementRight"].GetBool()),
            weight(d["payload"]["weight"].GetUint())
        {}
  

    
    PrivilegeGrantedEvent(const GrantPrivilegeCommand& c):
        id(c.id),
requesterId(c.requesterId),
memberId(c.memberId),
pollingRight(c.pollingRight),
votingRight(c.votingRight),
initiativeRight(c.initiativeRight),
managementRight(c.managementRight),
weight(c.weight)
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
              "memberId",
              JsonUtils::serializeUuid(memberId, d->GetAllocator()),
              d->GetAllocator());

          Value pollingRight_value;
          pollingRight_value.SetBool(pollingRight);
          payload.AddMember("pollingRight", pollingRight_value, d->GetAllocator());

          Value votingRight_value;
          votingRight_value.SetBool(votingRight);
          payload.AddMember("votingRight", votingRight_value, d->GetAllocator());

          Value initiativeRight_value;
          initiativeRight_value.SetBool(initiativeRight);
          payload.AddMember("initiativeRight", initiativeRight_value, d->GetAllocator());

          Value managementRight_value;
          managementRight_value.SetBool(managementRight);
          payload.AddMember("managementRight", managementRight_value, d->GetAllocator());

          Value weight_value;
          weight_value.SetUint(weight);
          payload.AddMember("weight", weight_value, d->GetAllocator());     

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
  