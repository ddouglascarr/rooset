#include "PrivilegeUtils.h"
#include "exceptions/CommandEvaluationException.h"

void rooset::PrivilegeUtils::assertManagementRight(const UnitAggregate& unit, const uuid& memberId)
{
  try {
    const auto requesterPrivileges = unit.getPrivileges().at(memberId);
    if (!requesterPrivileges.managementRight) {
      throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member does not have management rights");
    }
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member is not privilgegd for this unit");
  }
}

void rooset::PrivilegeUtils::assertVotingRight(const UnitAggregate & unit, const uuid & memberId)
{
  try {
    const auto requesterPrivileges = unit.getPrivileges().at(memberId);
    if (!requesterPrivileges.votingRight) {
      throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member does not have voting rights");
    }
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member is not privilgegd for this unit");
  }
}

void rooset::PrivilegeUtils::assertInitiativeRight(const UnitAggregate& unit, const uuid& memberId)
{
  try {
    const auto memberPrivileges = unit.getPrivileges().at(memberId);
    if (!memberPrivileges.initiativeRight) {
      throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member does not have initiative rights");
    }
  } catch (const std::out_of_range& e) {
    throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
        "Member is not privilgegd for this unit");
  }
}