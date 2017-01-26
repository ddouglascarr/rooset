#pragma once
#include "gmock/gmock.h"
#include <map>
#include <vector>
#include "EventRepository.h"
#include "JsonUtils.h"
#include "IdUtils.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using ::testing::Invoke;
using ::testing::_;



ACTION_P(LoadMockAggregateEvents, eventsMap)
{
  const uuid& aggregateId = arg0;
  if (eventsMap.find(aggregateId) == eventsMap.end()) {
    throw CommandEvaluationException(
        ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
        "There are no events for this aggregate");
  }

  vector<rapidjson::Document> events;
  for (const auto& sEvent : eventsMap.at(aggregateId))
  {
    events.push_back(JsonUtils::parse(sEvent));
  }
  return move(events);
}



ACTION_P(AssertMockAggregateDoesNotExist, eventsMap)
{
  const uuid& aggregateId = arg0;
  if (eventsMap.find(aggregateId) != eventsMap.end()) {
    throw CommandEvaluationException(
        ExceptionCode::CONFLICT_EXCEPTION,
        "This aggregate does have events, but it should not");
  }
}



namespace rooset {



class EventRepositoryMockImpl :
    public rooset::EventRepository
{
private:
  map<uuid, vector<string>> eventsMap;

public:
  MOCK_METHOD1(save, uuid(const ProjectEvent<rapidjson::Document>& e));
  MOCK_METHOD1(loadAggregateEvents,
      vector<rapidjson::Document>(const uuid& aggregateId));
  MOCK_METHOD1(assertAggregateDoesNotExist, void(const uuid& aggregateId));

  // For integration tests, you probs dont need / want to use this for unit tests
  void setMockEvents(vector<string> events)
  {
    for (const auto& e : events) {
      auto d = JsonUtils::parse(e);
      uuid aggregateId = IdUtils::parse(d["payload"]["id"].GetString());
      if (eventsMap.find(aggregateId) == eventsMap.end()) {
        eventsMap[aggregateId] = vector<string>();
      }
      eventsMap.at(aggregateId).push_back(e);
    }

    ON_CALL(*this, loadAggregateEvents(_))
        .WillByDefault(LoadMockAggregateEvents(eventsMap));

    ON_CALL(*this, assertAggregateDoesNotExist(_))
        .WillByDefault(AssertMockAggregateDoesNotExist(eventsMap));
  }

};




}
