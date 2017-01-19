
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

  class CreateNewInitiativeCommand : public ProjectCommand<Document>
  {
  private:
    const string MESSAGE_TYPE = "CREATE_NEW_INITIATIVE_COMMAND";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid requesterId;
        const uuid initiativeId;
        const uuid unitId;
        const uuid areaId;
        const uuid policyId;
        const string name;
        const bool polling;
        const string externalReference;
        const string content;
        const string textSearchData;
        const boost::posix_time::ptime created;

    
        CreateNewInitiativeCommand(
            uuid id,
            uuid requesterId,
            uuid initiativeId,
            uuid unitId,
            uuid areaId,
            uuid policyId,
            string name,
            bool polling,
            string externalReference,
            string content,
            string textSearchData,
            boost::posix_time::ptime created) :
            id(id),
            requesterId(requesterId),
            initiativeId(initiativeId),
            unitId(unitId),
            areaId(areaId),
            policyId(policyId),
            name(name),
            polling(polling),
            externalReference(externalReference),
            content(content),
            textSearchData(textSearchData),
            created(created)
        {}
  

    
        CreateNewInitiativeCommand(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            requesterId(JsonUtils::parseUuid(d["payload"]["requesterId"])),
            initiativeId(JsonUtils::parseUuid(d["payload"]["initiativeId"])),
            unitId(JsonUtils::parseUuid(d["payload"]["unitId"])),
            areaId(JsonUtils::parseUuid(d["payload"]["areaId"])),
            policyId(JsonUtils::parseUuid(d["payload"]["policyId"])),
            name(JsonUtils::parseString(d["payload"]["name"])),
            polling(d["payload"]["polling"].GetBool()),
            externalReference(JsonUtils::parseString(d["payload"]["externalReference"])),
            content(JsonUtils::parseString(d["payload"]["content"])),
            textSearchData(JsonUtils::parseString(d["payload"]["textSearchData"])),
            created(boost::posix_time::from_time_t(
                time_t(d["payload"]["created"].GetDouble())))
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

          Value initiativeId_value;
          string initiativeId_str = idTools->serialize(initiativeId);
          initiativeId_value.SetString(initiativeId_str.c_str(), initiativeId_str.size(), d->GetAllocator());
          payload.AddMember("initiativeId", initiativeId_value, d->GetAllocator());    

          Value unitId_value;
          string unitId_str = idTools->serialize(unitId);
          unitId_value.SetString(unitId_str.c_str(), unitId_str.size(), d->GetAllocator());
          payload.AddMember("unitId", unitId_value, d->GetAllocator());    

          Value areaId_value;
          string areaId_str = idTools->serialize(areaId);
          areaId_value.SetString(areaId_str.c_str(), areaId_str.size(), d->GetAllocator());
          payload.AddMember("areaId", areaId_value, d->GetAllocator());    

          Value policyId_value;
          string policyId_str = idTools->serialize(policyId);
          policyId_value.SetString(policyId_str.c_str(), policyId_str.size(), d->GetAllocator());
          payload.AddMember("policyId", policyId_value, d->GetAllocator());    

          payload.AddMember(
              "name",
              JsonUtils::serializeString(name, d->GetAllocator()),
              d->GetAllocator());

          Value polling_value;
          polling_value.SetBool(polling);
          payload.AddMember("polling", polling_value, d->GetAllocator());

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
          created_value.SetDouble(boost::posix_time::to_time_t(created));
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
  