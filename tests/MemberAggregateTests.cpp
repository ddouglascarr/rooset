#include "catch.hpp"

#include <string>
#include "framework/AggregateRepository.h"
#include "aggregates/member/MemberAggregate.h"
#include "aggregates/member/MemberCommandHandler.h"
#include "aggregates/member/commands/CreateAdminMemberCommand.h"
#include "aggregates/member/events/AdminMemberCreatedEvent.h"

using namespace rooset;
using namespace std;

namespace testing {
  // auto idTools = createIdToolsInstance();
}
SCENARIO("CreateNewAdminUserCommand")
{
  GIVEN("A Fresh System")
  {
    class MockAggregateRepository : public AggregateRepository<MemberAggregate>
    {
    public:
      unique_ptr<MemberAggregate> load(uuid id) const override
      {
        throw string("should not be called");
      }
      void assertAggregateDoesNotExist(uuid id) const override {}
    };

    MemberCommandHandler<MockAggregateRepository> commandHandler;

    WHEN("CreateAdminMemberCommand")
    {
      boost::uuids::random_generator idgen;
      auto idTools = make_unique<IdToolsImpl>();
      auto id = idTools->generateUniqueId();
      CreateAdminMemberCommand c(id, "admin", "pw1", "adam admin", "adam@admin.com");
      auto pe = commandHandler.evaluate(c);

      THEN("a AdminMemberCreatedEvent is emitted")
      {
        /*
        auto e = dynamic_cast<AdminMemberCreatedEvent&>(*pe);
        REQUIRE( e.id == id );
        AdminMemberCreatedEvent expectedEvent(c);
        REQUIRE(e.login == "admin");
        */
        Document expected;
        expected.SetObject();

        Value vId;
        string sId = idTools->serialize(id);
        vId.SetString(sId.c_str(), sId.size(), expected.GetAllocator());
        expected.AddMember("id", vId, expected.GetAllocator());
        expected.AddMember("login", "admin", expected.GetAllocator());
        expected.AddMember("password", "pw1", expected.GetAllocator());
        expected.AddMember("name", "adam admin", expected.GetAllocator());
        expected.AddMember("notifyEmail", "adam@admin.com", expected.GetAllocator());
        auto result = pe->serialize();
        REQUIRE(*result == expected);
      }
    }

  }
}

