#pragma once
#include <memory>
#include "framework/AggregateRepository.h"

using namespace std;

namespace rooset {

  template<typename Aggregate>
  class AggregateRepositoryEmptyMockImpl : public AggregateRepository<Aggregate>
  {
  public:
    unique_ptr<Aggregate> load(uuid id) const override
    {
      throw string("should not be called");
    }

    void assertAggregateDoesNotExist(uuid id) const override {}

  };

}