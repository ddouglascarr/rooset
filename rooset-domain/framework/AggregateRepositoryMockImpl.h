#pragma once
#include <memory>
#include "framework/AggregateRepository.h"

using namespace std;

namespace rooset {

  template<typename Aggregate>
  class AggregateRepositoryMockImpl : public AggregateRepository<Aggregate>
  {
  private:
    const Aggregate& aggregate;
  public:
    AggregateRepositoryMockImpl(const Aggregate& aggregate) : aggregate(aggregate) {}

    unique_ptr<Aggregate> load(uuid id) const override
    {
      return make_unique<Aggregate>(aggregate);
    }

    void assertAggregateDoesNotExist(uuid id) const override
    {
      throw string("aggregate exists");
    }
  };

}
