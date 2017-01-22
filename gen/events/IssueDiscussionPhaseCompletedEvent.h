
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
#include "commands/CompleteIssueDiscussionPhaseCommand.h"

using namespace std;
using namespace rooset;
using namespace rapidjson;

namespace rooset {

  class IssueDiscussionPhaseCompletedEvent : public ProjectEvent<Document>
  {
  private:
    const string MESSAGE_TYPE = "ISSUE_DISCUSSION_PHASE_COMPLETED_EVENT";

  public:
    static const SchemaDocument schema;

    const uuid id;

    
        IssueDiscussionPhaseCompletedEvent(
            uuid id) :
            id(id)
        {}
  

    
        IssueDiscussionPhaseCompletedEvent(const Document& d) :
        
            id(JsonUtils::parseUuid(d["payload"]["id"]))
        {}
  

    
    IssueDiscussionPhaseCompletedEvent(const CompleteIssueDiscussionPhaseCommand& c):
        id(c.id)
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

      d->AddMember("payload", payload, d->GetAllocator());

      return d;
    }

    string getEventType() override
    {
      return MESSAGE_TYPE;
    }

  };

};
  