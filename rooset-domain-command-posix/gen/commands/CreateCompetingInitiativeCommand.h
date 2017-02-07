
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

  class CreateCompetingInitiativeCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "CREATE_COMPETING_INITIATIVE_COMMAND";

  public:
    static const SchemaDocument schema;

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;
        const string name;
        const string externalReference;
        const string content;
        const string textSearchData;
        const unsigned int created;

    
        CreateCompetingInitiativeCommand(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            string name,
            string externalReference,
            string content,
            string textSearchData,
            unsigned int created) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId),
            name(name),
            externalReference(externalReference),
            content(content),
            textSearchData(textSearchData),
            created(created)
        {}
  

    
        CreateCompetingInitiativeCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            externalReference(JsonUtils::parseString(d["payload"]["externalReference"])),
            content(JsonUtils::parseString(d["payload"]["content"])),
            textSearchData(JsonUtils::parseString(d["payload"]["textSearchData"])),
            created(d["payload"]["created"].GetUint())
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

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "externalReference",
              JsonUtils::serializeString(externalReference, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "content",
              JsonUtils::serializeString(content, d->GetAllocator()),
              d->GetAllocator());

          payload.AddMember(
              "textSearchData",
              JsonUtils::serializeString(textSearchData, d->GetAllocator()),
              d->GetAllocator());

          Value created_value;
          created_value.SetUint(created);
          payload.AddMember("created", created_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() const override
    {
      return MESSAGE_TYPE;
    }

  };

};