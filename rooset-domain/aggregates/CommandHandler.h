#pragma once
#include <memory>
#include <vector>
#include "framework/AggregateRepositoryImpl.h"
#include "framework/EventRepository.h"
#include "UnitCommandHandler.h"
#include "UnitAggregate.h"
#include "MemberCommandHandler.h"
#include "MemberAggregate.h"
#include "IssueCommandHandler.h"
#include "IssueAggregate.h"



using namespace std;
namespace rooset {



class CommandHandler :
    public UnitCommandHandler,
    public IssueCommandHandler,
    public MemberCommandHandler
{
public:
  using UnitCommandHandler::evaluate;
  using IssueCommandHandler::evaluate;
  using MemberCommandHandler::evaluate;

  virtual ~CommandHandler() {}

  CommandHandler(const shared_ptr<EventRepository>& eventRepository) :
      UnitCommandHandler(make_unique<AggregateRepositoryImpl<UnitAggregate>>(eventRepository)),
      IssueCommandHandler(
          make_unique<AggregateRepositoryImpl<IssueAggregate>>(eventRepository),
          make_unique<AggregateRepositoryImpl<UnitAggregate>>(eventRepository)),
      MemberCommandHandler(make_unique<AggregateRepositoryImpl<MemberAggregate>>(eventRepository))
  {}
};



}
