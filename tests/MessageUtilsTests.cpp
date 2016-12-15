#include "gtest/gtest.h"

#include <iostream>
#include <memory>
#include "framework/MessageUtils.h"
#include "aggregates/UnitAggregate.h"
#include "events/UnitCreatedEvent.h"
#include "framework/IdToolsImpl.h"
#include "events/UnitDelegationSetEvent.h"
#include "commands/SetUnitDelegationCommand.h"
#include "exceptions/CommandEvaluationException.h"

using namespace std;
using namespace rooset;

namespace roosetMessageUtilsTests {

  auto idTools = make_unique<IdToolsImpl>();

  TEST(MessageUtils, handleEventShouldCallHasEvent)
  {
    bool has = false;
    bool hasnot = false;

    const UnitCreatedEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    auto a = make_unique<UnitAggregate>(e);

    const UnitDelegationSetEvent e1{ idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId() };
    MessageUtils::handleEvent<UnitAggregate, UnitDelegationSetEvent>(
        *a, e1, [&]() { hasnot = true; });
    EXPECT_EQ(has, false);
    EXPECT_EQ(hasnot, false);
    EXPECT_EQ(a->getDelegations().size(), 1);
  }

  TEST(MessageUtils, handleEventShouldCallMissingEvent)
  {
    bool has = false;
    bool hasnot = false;

    const UnitCreatedEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    UnitAggregate a(e);

    SetUnitDelegationCommand c1{idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId()};
    MessageUtils::handleEvent<UnitAggregate, SetUnitDelegationCommand>(
      a, c1, [&]() { hasnot = true; });

    EXPECT_EQ(has, false);
    EXPECT_EQ(hasnot, true);
    EXPECT_EQ(a.getDelegations().size(), 0);
  }

  TEST(MessageUtils, instantiateAggregateShouldInstantiate)
  {
    bool hasnot = false;

    const UnitCreatedEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    const auto a = MessageUtils::instantiateAggregate<UnitAggregate, UnitCreatedEvent>(e, [&]() {hasnot = true; });

    EXPECT_FALSE(a == nullptr);
    EXPECT_FALSE(hasnot);
  }

  TEST(MessageUtils, instantiateAggregateShouldNotInstantiate)
  {
    bool hasnot = false;

    SetUnitDelegationCommand c1{ idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId() };
    const auto a = MessageUtils::instantiateAggregate<UnitAggregate, SetUnitDelegationCommand>(c1, [&]() {hasnot = true; });

    EXPECT_TRUE(a == nullptr);
    EXPECT_TRUE(hasnot);
  }

  TEST(MessageUtils, applyEventShouldReturnNullptrIfNotMatched)
  {
    bool hasnot = false;

    SetUnitDelegationCommand c1{ idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId() };
    unique_ptr<UnitAggregate> a = nullptr;
    MessageUtils::applyEvent<UnitAggregate, SetUnitDelegationCommand>(a, c1, [&]() {hasnot = true; });
    
    EXPECT_EQ(a, nullptr);
    EXPECT_TRUE(hasnot);
  }

  TEST(MessageUtils, applyEventShouldReturnInstantiatedEvent)
  {
    bool hasnot = false;

    const UnitCreatedEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    unique_ptr<UnitAggregate> a = nullptr;
    MessageUtils::applyEvent<UnitAggregate, UnitCreatedEvent>(a, e, [&]() {hasnot = true; });

    EXPECT_FALSE(a == nullptr);
    EXPECT_FALSE(hasnot);
  }

  TEST(MessageUtils, applyEventShouldCallHandleEvent)
  {
    bool hasnot = false;

    const UnitCreatedEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    const UnitDelegationSetEvent e1{ idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId() };

    unique_ptr<UnitAggregate> a = nullptr;
    MessageUtils::applyEvent<UnitAggregate, UnitCreatedEvent>(a, e, [&]() {hasnot = true; });
    MessageUtils::applyEvent<UnitAggregate, UnitDelegationSetEvent>(a, e1, [&]() {hasnot = true; });

    EXPECT_FALSE(a == nullptr);
    EXPECT_FALSE(hasnot);
    EXPECT_EQ(a->getDelegations().size(), 1);
  }

  TEST(MessageUtils, applyEventShouldThrowIfHandlableEventBeforeConstructor)
  {
    const UnitDelegationSetEvent e{ idTools->generateUniqueId(), idTools->generateUniqueId(), idTools->generateUniqueId() };

    try {
      unique_ptr<UnitAggregate> a = nullptr;
      MessageUtils::applyEvent<UnitAggregate, UnitDelegationSetEvent>(a, e, []() {});
      throw "should have thrown";
    } catch (const CommandEvaluationException& e) {
      EXPECT_EQ(e.getExceptionCode(), ExceptionCode::CONFLICT_EXCEPTION);
    }
  }

  TEST(MessageUtils, applyEventShouldThrowIfConstructorEventFollowsOtherConstructorEvent)
  {

    const UnitCreatedEvent e0{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };
    const UnitCreatedEvent e1{ idTools->generateUniqueId(), idTools->generateUniqueId(), "foo", "foobar" };

    unique_ptr<UnitAggregate> a = nullptr;
    MessageUtils::applyEvent<UnitAggregate, UnitCreatedEvent>(a, e0, []() {});
    try {
      MessageUtils::applyEvent(a, e1, []() {});
      throw "should have thrown";
    } catch (const CommandEvaluationException& e) {
      EXPECT_EQ(e.getExceptionCode(), ExceptionCode::CONFLICT_EXCEPTION);
    }
  }

}