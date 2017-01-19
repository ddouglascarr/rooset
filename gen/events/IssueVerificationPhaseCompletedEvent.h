
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

  class IssueVerificationPhaseCompletedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "ISSUE_VERIFICATION_PHASE_COMPLETED_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const vector<uuid> passingInitiatives;

    
        IssueVerificationPhaseCompletedEvent(
            uuid id,
            vector<uuid> passingInitiatives) :
            id(id),
            passingInitiatives(passingInitiatives)
        {}
  

    
        IssueVerificationPhaseCompletedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"])),
            passingInitiatives(JsonUtils::parseArray<uuid>(
                d["payload"]["passingInitiatives"],
                [&](const rapidjson::Value& v) { return JsonUtils::parseUuid(v); }))
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

          auto passingInitiatives_value = JsonUtils::serializeArray<uuid>(
              passingInitiatives,
              [](const uuid& id, rapidjson::Document::AllocatorType& allocator) {
                return JsonUtils::serializeUuid(id, allocator);
              },
              d->GetAllocator());
          payload.AddMember("passingInitiatives", passingInitiatives_value, d->GetAllocator());
    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  