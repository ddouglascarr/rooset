#include "gtest/gtest.h"

#include <string>
#include <memory>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o
#include "commands/CreateAdminMemberCommand.h"
#include "events/AdminMemberCreatedEvent.h"
#include "ratk/JsonUtils.h"
#include "ratk/IdToolsImpl.h"

// using namespace rooset;
using namespace std;
using namespace boost;

namespace roosetTesting {

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

  class MyTestFixture : public ::testing::Test
  {
  protected:
    const string d1 = "foo";
    string setByConstructor;
    string setBySetUp;
    string setMe;

    MyTestFixture()
    {
      setByConstructor = "bar";
    }

    virtual void SetUp()
    {
      setBySetUp = "bing";
    }
  };
  

  TEST_F(MyTestFixture, getDataFromFixture)
  {
    EXPECT_EQ(d1, "foo");
    EXPECT_EQ(setByConstructor, "bar");
    EXPECT_EQ(setBySetUp, "bing");
    setMe = "foobar";
  }

  TEST_F(MyTestFixture, dataIsNotSharedBetweenTests)
  {
    EXPECT_EQ(setMe, "");
  }

}
