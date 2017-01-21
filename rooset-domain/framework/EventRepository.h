#pragma once
#include <vector>
#include "rapidjson/document.h"
#include "ProjectEvent.h"
#include "uuid.h"


namespace rooset {

  class EventRepository
  {
  public:
    virtual uuid save(const ProjectEvent<rapidjson::Document>& e) =0;
    virtual vector<ProjectEvent<rapidjson::Document>> loadAggregateEvents(
        const uuid& aggregateId) =0;
  };

}
