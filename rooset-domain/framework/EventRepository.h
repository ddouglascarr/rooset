#pragma once
#include <vector>
#include "rapidjson/document.h"
#include "framework/ProjectEvent.h"
#include "uuid.h"


namespace rooset {

  class EventRepository
  {
  public:
    virtual uuid save(const ProjectEvent<rapidjson::Document>& e) =0;
    virtual vector<rapidjson::Document> loadAggregateEvents(
        const uuid& aggregateId) =0;
    virtual void assertAggregateDoesNotExist(const uuid& aggregateId) =0;
  };

}
