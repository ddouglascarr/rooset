#pragma once
#include <memory>
#include <vector>
#include "AggregateRepositoryMockImpl.h"
#include "aggregates/UnitCommandHandler.h"
#include "aggregates/UnitAggregate.h"
#include "aggregates/MemberCommandHandler.h"
#include "aggregates/MemberAggregate.h"


using namespace std;
namespace rooset {

  class CommandHandlerTestImpl : 
      public UnitCommandHandler,
      public MemberCommandHandler  
  {
  public:
    using UnitCommandHandler::evaluate;
    using MemberCommandHandler::evaluate;

    CommandHandlerTestImpl(const vector<unique_ptr<rapidjson::Document>>& events) :
        UnitCommandHandler(make_unique<AggregateRepositoryMockImpl<UnitAggregate>>(events)),
        MemberCommandHandler(make_unique<AggregateRepositoryMockImpl<MemberAggregate>>(events))
    {}
  };
}