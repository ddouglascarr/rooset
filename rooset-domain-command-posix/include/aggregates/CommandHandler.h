#pragma once
#include <memory>
#include <vector>
#include "ratk/AggregateRepositoryImpl.h"
#include "ratk/EventRepository.h"
#include "UnitCommandHandler.h"
#include "UnitAggregate.h"
#include "IssueCommandHandler.h"
#include "IssueAggregate.h"



using namespace std;
namespace rooset {



class CommandHandler :
    public UnitCommandHandler,
    public IssueCommandHandler
{
public:
  using UnitCommandHandler::evaluate;
  using IssueCommandHandler::evaluate;

  virtual ~CommandHandler() {}

  CommandHandler(const shared_ptr<EventRepository>& eventRepository) :
      UnitCommandHandler(make_unique<AggregateRepositoryImpl<UnitAggregate>>(eventRepository)),
      IssueCommandHandler(
          make_unique<AggregateRepositoryImpl<IssueAggregate>>(eventRepository),
          make_unique<AggregateRepositoryImpl<UnitAggregate>>(eventRepository))
  {}
};



}
