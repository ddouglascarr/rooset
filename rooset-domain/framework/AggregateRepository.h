#pragma once


namespace rooset {
  template<typename Aggregate>
  class AggregateRepository
  {
  public:
    virtual Aggregate load(string id) const = 0;
  };
}
