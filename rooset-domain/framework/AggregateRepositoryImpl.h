#pragma once
#include <memory>
#include "AggregateRepository.h"
#include "IdUtils.h"
#include "EventRepository.h"
#include "events/EventUtils.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;

namespace rooset {



template<typename Aggregate>
class AggregateRepositoryImpl :
    public AggregateRepository<Aggregate>
{
private:
  shared_ptr<EventRepository> eventRepository;

public:
  inline AggregateRepositoryImpl(shared_ptr<EventRepository> eventRepository) :
      eventRepository(eventRepository)
  {}

  unique_ptr<Aggregate> load(uuid id) const override
  {
    unique_ptr<Aggregate> aggregate = nullptr;

    // TODO: [OPTIMISATION] eventRepository->forEachAggregateEvent(id, [](Document) { apply(d) })
    vector<rapidjson::Document> events = eventRepository->loadAggregateEvents(
        id);
    for (const auto& e : events) {
      EventUtils::applyEvent<Aggregate>(aggregate, e, [&]() {
        throw CommandEvaluationException(
            ExceptionCode::INVALID_INPUT_EXCEPTION,
            R"err(
  The events being applied to this aggregate are for a different aggregate.
  - Check the payload.id property of the command is for the correct aggregate.
  - If this is occuring in a test, check the given events payload.id property.
  - If this is in production and the commands payload.id property is correct,
      the event stream may be corrupted. This would be bad.
                )err");
      });
    }

    if (aggregate == nullptr) throw runtime_error(
        "aggregate nullptr error");
    return aggregate;
  }

  void assertAggregateDoesNotExist(uuid id) const override
  {
    eventRepository->assertAggregateDoesNotExist(id);
  }

};



}
