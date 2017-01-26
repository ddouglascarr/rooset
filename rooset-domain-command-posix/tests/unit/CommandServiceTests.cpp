#include "gmock/gmock.h"

#include <stdexcept>
#include "ratk/EventRepositoryMockImpl.h"
#include "ratk/IdToolsImpl.h"
#include "ratk/IdUtils.h"
#include "ratk/JsonUtils.h"
#include "commands/UnsetIssueBallotCommand.h"
#include "events/IssueBallotUnsetEvent.h"
#include "aggregates/CommandHandler.h"
#include "ratk/CommandServiceImpl.h"


using namespace std;
using namespace rooset;
using ::testing::Exactly;
using ::testing::Return;
using ::testing::ByMove;
using ::testing::_;



class CommandHandlerMockImpl :
    public rooset::CommandHandler
{
public:
  using CommandHandler::evaluate;

  CommandHandlerMockImpl(const shared_ptr<EventRepository>& eventRepository) :
      rooset::CommandHandler(eventRepository)
  {}

  MOCK_METHOD1(evaluate, unique_ptr<ProjectEvent<Document>>(
      const UnsetIssueBallotCommand& c));

};



class CommandServiceImplTests:
  public ::testing::Test
{
protected:
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<EventRepositoryMockImpl>();
  shared_ptr<CommandHandlerMockImpl> commandHandler = make_shared<CommandHandlerMockImpl>(
      eventRepository);
  CommandServiceImpl<CommandHandlerMockImpl> commandService;

public:
  CommandServiceImplTests() : commandService(eventRepository, *commandHandler)
  {
  }
};


MATCHER_P(docIsSerializedTo, c, "")
{
  return *(std::get<0>(arg).serialize()) == *(c.serialize());
};



TEST(CommandServiceTests, testMocks)
{
  IdToolsImpl idTools;
  uuid id = idTools.generateUniqueId();
  uuid memberId = idTools.generateUniqueId();

  UnsetIssueBallotCommand cmd0(id, memberId);
  UnsetIssueBallotCommand cmd1(id, memberId);

  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      EventRepositoryMockImpl>();
  CommandHandlerMockImpl commandHandler(eventRepository);

  EXPECT_CALL(*eventRepository, loadAggregateEvents(id))
      .Times(Exactly(1));

  EXPECT_CALL(commandHandler, evaluate(_))
      .With(docIsSerializedTo(cmd1))
          .Times(Exactly(1))
      .WillOnce(Return(ByMove(make_unique<IssueBallotUnsetEvent>(id, memberId))));

  eventRepository->loadAggregateEvents(id);
  auto evt = commandHandler.evaluate(cmd0);
  IssueBallotUnsetEvent expectedEvt(id, memberId);
  cout << JsonUtils::serialize(*evt->serialize());
  EXPECT_EQ(*evt->serialize(), *expectedEvt.serialize());
}



TEST_F(CommandServiceImplTests, shouldThrowOnInvalidJson)
{
  try {
    commandService.evaluate("{invalidjson");
    throw std::runtime_error("should have thrown");
  } catch (const CommandEvaluationException& e) {
    EXPECT_EQ(e.getExceptionCode(), ExceptionCode::INVALID_INPUT_EXCEPTION);
  }
}



TEST_F(CommandServiceImplTests, shouldSaveEvent)
{
  auto issueId = IdUtils::parse("464b1ebb-32c1-460c-8e9e-111111111111");
  auto memberId = IdUtils::parse("464b1ebb-32c1-460c-8e9e-222222222222");
  auto eventId = IdUtils::parse("464b1ebb-32c1-460c-8e9e-333333333333");
  UnsetIssueBallotCommand cmd(issueId, memberId);
  IssueBallotUnsetEvent expectedEvt(issueId, memberId);
  auto sJson = JsonUtils::serialize(*cmd.serialize());

  EXPECT_CALL(*commandHandler, evaluate(_))
      .With(docIsSerializedTo(cmd))
          .Times(Exactly(1))
      .WillOnce(Return(ByMove(make_unique<IssueBallotUnsetEvent>(issueId, memberId))));

  EXPECT_CALL(*eventRepository, save(_))
      .With(docIsSerializedTo(expectedEvt))
          .Times(Exactly(1))
      .WillOnce(Return(eventId));

  auto returnedId = commandService.evaluate(sJson);
  EXPECT_EQ(returnedId, eventId);
}
