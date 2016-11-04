
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

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class UnitDelegationUnsetEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "UNIT_DELEGATION_UNSET_EVENT";

  public:
    static const unique_ptr<SchemaDocument> schema;
    const unique_ptr<IdTools> idTools = make_unique<IdToolsImpl>();

    const uuid id;
        const uuid trusterId;

    
        UnitDelegationUnsetEvent(
            uuid id,
            uuid trusterId) :
            id(id) ,
            trusterId(trusterId) 
        {}
  

    
        UnitDelegationUnsetEvent(const Document& d) :
        
            id(idTools->parse(string(
                d["payload"]["id"].GetString(),
                d["payload"]["id"].GetStringLength()))),
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
  