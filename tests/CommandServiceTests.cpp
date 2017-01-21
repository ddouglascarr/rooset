#include "gmock/gmock.h"

#include <stdexcept>
#include "framework/EventRepository.h"
#include "framework/IdToolsImpl.h"
#include "framework/IdUtils.h"
#include "framework/JsonUtils.h"
#include "commands/UnsetIssueBallotCommand.h"
#include "events/IssueBallotUnsetEvent.h"
#include "framework/CommandServiceImpl.h"
#include "framework/CommandHandlerTestImpl.h"


using namespace std;
using namespace rooset;
using ::testing::Exactly;
using ::testing::Return;
using ::testing::ByMove;
using ::testing::_;



class EventRepositoryMockImpl :
    public rooset::EventRepository
{
public:
  MOCK_METHOD1(save, uuid(const ProjectEvent<rapidjson::Document>& e));
  MOCK_METHOD1(loadAggregateEvents, vector<ProjectEvent<rapidjson::Document>>(
      const uuid& aggregateId));
};



class CommandHandlerMockImpl :
    public rooset::CommandHandlerTestImpl
{
public:
  using CommandHandlerTestImpl::evaluate;

  CommandHandlerMockImpl() :
      rooset::CommandHandlerTestImpl(vector<unique_ptr<rapidjson::Document>>())
  {}

  MOCK_METHOD1(evaluate, unique_ptr<ProjectEvent<Document>>(
      const UnsetIssueBallotCommand& c));

};



class CommandServiceImplTests:
  public ::testing::Test
{
protected:
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<EventRepositoryMockImpl>();
  shared_ptr<CommandHandlerMockImpl> commandHandler = make_shared<CommandHandlerMockImpl>();
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

  EventRepositoryMockImpl eventRepository;
  CommandHandlerMockImpl commandHandler;

  EXPECT_CALL(eventRepository, loadAggregateEvents(id))
      .Times(Exactly(1));

  EXPECT_CALL(commandHandler, evaluate(_))
      .With(docIsSerializedTo(cmd1))
          .Times(Exactly(1))
      .WillOnce(Return(ByMove(make_unique<IssueBallotUnsetEvent>(id, memberId))));

  eventRepository.loadAggregateEvents(id);
  auto evt = commandHandler.evaluate(cmd0);
  IssueBallotUnsetEvent expectedEvt(id, memberId);
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

  auto returnedId = commandService.evaluate(*sJson);
  EXPECT_EQ(returnedId, eventId);
}
