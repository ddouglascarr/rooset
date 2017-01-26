#include "gmock/gmock.h"
#include "ratk/AggregateRepositoryImpl.h"
#include "ratk/EventRepositoryMockImpl.h"
#include "ratk/IdUtils.h"
#include "aggregates/UnitAggregate.h"
#include "aggregates/CommandHandler.h"

using namespace rooset;
using namespace std;
using ::testing::NiceMock;
using ::testing::Exactly;
using ::testing::Return;
using ::testing::Throw;
using ::testing::ByMove;
using ::testing::_;



class AggregateRepositoryImplTests :
    public ::testing::Test
{
protected:
  shared_ptr<EventRepositoryMockImpl> eventRepository = make_shared<
      NiceMock<EventRepositoryMockImpl>>();
  AggregateRepositoryImpl<UnitAggregate> aggregateRepository;

public:
  AggregateRepositoryImplTests() :
      aggregateRepository(eventRepository)
  {}
};



TEST_F(AggregateRepositoryImplTests,
    eventRepositoryMockWorks)
{
  vector<string> sEvents;
  sEvents.push_back(u8R"json({
    "type": "UNIT_CREATED_EVENT",
    "payload": {
      "id": "464b1ebb-32c1-460c-8e9e-111111111111",
      "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
      "name": "Test Unit",
      "description": "The Test Unit"
    }
  })json");

  eventRepository->setMockEvents(sEvents);
  CommandHandler commandHandler(eventRepository);
  auto cmd_doc = JsonUtils::parse(u8R"json({
  "type": "GRANT_PRIVILEGE_COMMAND",
  "payload": {
    "id": "464b1ebb-32c1-460c-8e9e-111111111111",
    "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
    "memberId": "f00b6b62-72d1-4c5e-974d-8a678f2c252c",
    "pollingRight": true,
    "votingRight": true,
    "initiativeRight": true,
    "managementRight": true,
    "weight": 1
  }
})json");

  GrantPrivilegeCommand cmd(cmd_doc);
  auto evt = commandHandler.evaluate(cmd);
  EXPECT_EQ(string((*evt->serialize())["payload"]["id"].GetString()),
      "464b1ebb-32c1-460c-8e9e-111111111111");
}



TEST_F(AggregateRepositoryImplTests,
    assertAggregateDoesNotExist_callEventRepository)
{
  uuid id = IdUtils::parse("464b1ebb-32c1-460c-8e9e-111111111111");
  EXPECT_CALL(*eventRepository, assertAggregateDoesNotExist(id))
      .Times(Exactly(1));
  aggregateRepository.assertAggregateDoesNotExist(id);
}



TEST_F(AggregateRepositoryImplTests,
    assertAggregateDoesNotExist_shouldPropogateProjectErr)
{
  uuid id = IdUtils::parse("464b1ebb-32c1-460c-8e9e-111111111111");
  ON_CALL(*eventRepository, assertAggregateDoesNotExist(id))
      .WillByDefault(Throw(CommandEvaluationException(
          ExceptionCode::CONFLICT_EXCEPTION,
          "some message")));

  try {
    aggregateRepository.assertAggregateDoesNotExist(id);
    throw runtime_error("should have thrown");
  } catch (const CommandEvaluationException& e) {
    EXPECT_EQ(e.getExceptionCode(), ExceptionCode::CONFLICT_EXCEPTION);
  }
}



TEST_F(AggregateRepositoryImplTests, load_shouldReturnAggregates)
{
  uuid id = IdUtils::parse("464b1ebb-32c1-460c-8e9e-111111111111");

  vector<rapidjson::Document> mockEvents;
  mockEvents.push_back(JsonUtils::parse(u8R"json({
    "type": "UNIT_CREATED_EVENT",
    "payload": {
      "id": "464b1ebb-32c1-460c-8e9e-111111111111",
      "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
      "name": "Test Unit",
      "description": "The Test Unit"
    }
  })json"));
  mockEvents.push_back(JsonUtils::parse(u8R"json({
    "type": "AREA_CREATED_EVENT",
    "payload": {
      "id": "464b1ebb-32c1-460c-8e9e-111111111111",
      "requesterId": "464b1ebb-32c1-460c-8e9e-222222222222",
      "areaId": "464b1ebb-32c1-460c-8e9e-333333333333",
      "name": "test area",
      "description": "the test area",
      "externalReference": "area.com"
    }
  })json"));

  EXPECT_CALL(*eventRepository, loadAggregateEvents(id))
      .Times(Exactly(1))
      .WillOnce(Return(ByMove(move(mockEvents))));

  auto returnedAggregate = aggregateRepository.load(id);
  EXPECT_EQ(returnedAggregate->getAreas().size(), 1);
}
