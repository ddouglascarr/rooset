#pragma once
#include "uuid.h"

using namespace rooset;


namespace rooset {
  template<typename Aggregate>
  class AggregateRepository
  {
  public:
    virtual unique_ptr<Aggregate> load(uuid id) const = 0;
    virtual void assertAggregateDoesNotExist(uuid id) const = 0;
  };
}
