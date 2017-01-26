#pragma once
#include "ratk/uuid.h"
#include "UnitAggregate.h"

namespace rooset {

  class PrivilegeUtils
  {
  public:
    static void assertManagementRight(const UnitAggregate& unit, const uuid& memberId);
    static void assertVotingRight(const UnitAggregate& unit, const uuid& memberId);
    static void assertInitiativeRight(const UnitAggregate& unit, const uuid& memberId);
  };

}
