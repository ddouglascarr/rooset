#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o
#include "aggregates/MemberAggregate.h"
#include "aggregates/MemberCommandHandler.h"
#include "commands/CreateAdminMemberCommand.h"
#include "events/AdminMemberCreatedEvent.h"
#include "framework/AggregateRepositoryMockImpl.h"
#include "framework/AggregateRepositoryEmptyMockImpl.h"
#include "framework/JsonUtils.h"
#include "framework/IdToolsImpl.h"

// using namespace rooset;
using namespace std;
using namespace boost;

namespace test {

  auto idTools = make_unique<IdToolsImpl>();
  auto id = idTools->generateUniqueId();
  string idString = idTools->serialize(id);
  CreateAdminMemberCommand c(id, "admin", "pw1", "adam admin", "adam@admin.com");

  TEST(dependencies, boost_posix_time)
  {
    const auto testTime = 1477705463L;
    posix_time::ptime testPosixTime = posix_time::from_time_t(time_t(testTime));
    time_t resultTime = posix_time::to_time_t(testPosixTime);
    EXPECT_EQ(resultTime, testTime);
  }

  TEST(dependencies, boost_posix_duration)
  {
    const posix_time::time_duration testDuration = posix_time::seconds(600L);
    EXPECT_EQ(testDuration.minutes(), 10);
  }

  TEST(dependencies, map_in_struct)
  {
    struct Area {
      uuid id;
      map<uuid, uuid> delegations;
    };

    Area a{ id, map<uuid, uuid>() };
    a.delegations[id] = id;
    EXPECT_EQ(id, a.delegations.at(id));
    const auto isIdPresent = a.delegations.find(id) != a.delegations.end();
    const auto isAnotherIdPresent = a.delegations.find(idTools->generateUniqueId()) != a.delegations.end();
    EXPECT_EQ(isIdPresent, true);
    EXPECT_EQ(isAnotherIdPresent, false);
  }

  TEST(MemberAggregateTest, AdminMemberCreated)
  {
    auto repo = make_unique<AggregateRepositoryEmptyMockImpl<MemberAggregate>>();
    MemberCommandHandler commandHandler(move(repo));
    auto pe = commandHandler.evaluate(c);

    AdminMemberCreatedEvent expected{ id, "admin", "pw1", "adam admin", "adam@admin.com" };
    auto result = pe->serialize();

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    result->Accept(writer);
    auto resultStr = string(buffer.GetString(), buffer.GetSize());

    StringBuffer expectedBuffer;
    Writer<StringBuffer> expectedWriter(expectedBuffer);
    auto expectedDoc = expected.serialize();
    expectedDoc->Accept(expectedWriter);
    auto expectedStr = string(expectedBuffer.GetString(), expectedBuffer.GetSize());

    EXPECT_EQ(resultStr, expectedStr);
    EXPECT_EQ(*result, *expected.serialize());
  }

  TEST(MemberAggregateTest, MemberAggregateCreatedJson)
  {
    auto repo = make_unique<AggregateRepositoryEmptyMockImpl<MemberAggregate>>();
    MemberCommandHandler commandHandler(move(repo));
    auto pe = commandHandler.evaluate(c);

    const string j = "{\"type\":\"ADMIN_MEMBER_CREATED_EVENT\",\"payload\":{\"id\":\"" + idString + "\",\"login\":\"admin\",\"password\":\"pw1\",\"name\":\"adam admin\",\"notifyEmail\":\"adam@admin.com\"}}";
    // Document expectedJson;
    // expectedJson.Parse(j.c_str());
    auto expectedJson = JsonUtils::parse(j);

    AdminMemberCreatedEvent expected(*expectedJson);
    auto result = pe->serialize();

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    result->Accept(writer);
    auto resultStr = string(buffer.GetString(), buffer.GetSize());

    StringBuffer expectedBuffer;
    Writer<StringBuffer> expectedWriter(expectedBuffer);
    auto expectedDoc = expected.serialize();
    expectedDoc->Accept(expectedWriter);
    auto expectedStr = string(expectedBuffer.GetString(), expectedBuffer.GetSize());

    bool isPass = *result == *expected.serialize();
    if (isPass) {
      EXPECT_EQ(*result, *expected.serialize());
    } else {
      EXPECT_EQ(resultStr, expectedStr);
    }
  }

  
  TEST(MemberAggregateTest, MemberPasswordUpdated)
  {
    MemberAggregate mockMember(AdminMemberCreatedEvent{
        id, "foo", "pw1", "adam admin", "adam@admin.com" });
    auto repo = make_unique<AggregateRepositoryMockImpl<MemberAggregate>>(mockMember);
    MemberCommandHandler commandHandler(move(repo));
    UpdateMemberPasswordCommand cmd{ id, id, "pw1", "pw2" };
    auto pe = commandHandler.evaluate(cmd);
    MemberPasswordUpdatedEvent expected{ id, id, "pw2" };

    bool isPass = *pe->serialize() == *expected.serialize();
    if (isPass) {
      EXPECT_EQ(*pe->serialize(), *expected.serialize());
    } else {
      EXPECT_EQ(*JsonUtils::serialize(*pe->serialize()),
        *JsonUtils::serialize(*expected.serialize()));
    }
  }
}