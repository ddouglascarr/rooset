#pragma once
#include <memory>
#include <vector>
#include "framework/AggregateRepository.h"
#include "events/EventUtils.h"
#include "rapidjson/document.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;

namespace rooset {

  template<typename Aggregate>
  class AggregateRepositoryMockImpl : public AggregateRepository<Aggregate>
  {
  private:
    unique_ptr<Aggregate> aggregate;
    string aggregateId = "";
  public:
    AggregateRepositoryMockImpl(const vector<unique_ptr<rapidjson::Document>>& events)
    {
      aggregate = nullptr;
      for (const auto& e : events) {
        bool isForThisAggregate = true;
        EventUtils::applyEvent<Aggregate>(aggregate, *e, [&]() { isForThisAggregate = false; });
        if (isForThisAggregate) {
          string payloadId = (*e)["payload"]["id"].GetString();
          if (aggregateId == "") {
            aggregateId = payloadId;
          } 
          if (aggregateId != payloadId) {
            throw string("AggregateRepositoryMockImpl only supports one aggregateId per implementation. Check that all ids for an aggregate in your test match");
          }
        }
      }
    }

    unique_ptr<Aggregate> load(uuid id) const override
    {
      if (aggregate == nullptr) {
        throw CommandEvaluationException(ExceptionCode::ITEM_NOT_FOUND_EXCEPTION,
            "Aggregate has not been initialized");
      }
      return make_unique<Aggregate>(*aggregate);
    }

    void assertAggregateDoesNotExist(uuid id) const override
    {
      if (aggregate != nullptr) {
        throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION,
            "Aggregate Exists");
      }
    }
  };

}
