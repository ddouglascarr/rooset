
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


using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class DelegationBlockedForAreaEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "DELEGATION_BLOCKED_FOR_AREA_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid areaId;
        const uuid trusterId;

    
        DelegationBlockedForAreaEvent(
            uuid id,
            uuid areaId,
            uuid trusterId) :
            id(id) ,
            areaId(areaId) ,
            trusterId(trusterId) 
        {}
  

    
        DelegationBlockedForAreaEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
            areaId(idTools->parse(string(
                d["payload"]["areaId"].GetString(),
                d["payload"]["areaId"].GetStringLength()))),
            trusterId(idTools->parse(string(
                d["payload"]["trusterId"].GetString(),
                d["payload"]["trusterId"].GetStringLength())))
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

          Value trusterId_value;
          string trusterId_str = idTools->serialize(trusterId);
          trusterId_value.SetString(trusterId_str.c_str(), trusterId_str.size(), d->GetAllocator());
          payload.AddMember("trusterId", trusterId_value, d->GetAllocator());    

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  