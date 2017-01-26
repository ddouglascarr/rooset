
#include "enums/EnumUtils.h"
#include "enums/IssueState.h"
#include <stdexcept>

namespace rooset {
  IssueState EnumUtils::parseIssueState(const string& e)
  {
    if (e == "ADMISSION") return IssueState::ADMISSION;
if (e == "DISCUSSION") return IssueState::DISCUSSION;
if (e == "VERIFICATION") return IssueState::VERIFICATION;
if (e == "VOTING") return IssueState::VOTING;
if (e == "CANCELED_BY_ADMIN") return IssueState::CANCELED_BY_ADMIN;
if (e == "CANCELED_REVOKED_BEFORE_ACCEPTED") return IssueState::CANCELED_REVOKED_BEFORE_ACCEPTED;
if (e == "CANCELED_ISSUE_NOT_ACCEPTED") return IssueState::CANCELED_ISSUE_NOT_ACCEPTED;
if (e == "CANCELED_AFTER_REVOCATION_DURING_DISCUSSION") return IssueState::CANCELED_AFTER_REVOCATION_DURING_DISCUSSION;
if (e == "CANCELED_AFTER_REVOCATION_DURING_VERIFICATION") return IssueState::CANCELED_AFTER_REVOCATION_DURING_VERIFICATION;
if (e == "CANCELED_NO_INITIATIVE_ADMITTED") return IssueState::CANCELED_NO_INITIATIVE_ADMITTED;
if (e == "FINISHED_TABULATING_RESULTS") return IssueState::FINISHED_TABULATING_RESULTS;
if (e == "FINISHED_WITHOUT_WINNER") return IssueState::FINISHED_WITHOUT_WINNER;
if (e == "FINISHED_WITH_WINNER") return IssueState::FINISHED_WITH_WINNER;
    throw std::invalid_argument("unknown enum");
  }

  string EnumUtils::serializeIssueState(IssueState e)
  {
    if (e == IssueState::ADMISSION) return "ADMISSION";
if (e == IssueState::DISCUSSION) return "DISCUSSION";
if (e == IssueState::VERIFICATION) return "VERIFICATION";
if (e == IssueState::VOTING) return "VOTING";
if (e == IssueState::CANCELED_BY_ADMIN) return "CANCELED_BY_ADMIN";
if (e == IssueState::CANCELED_REVOKED_BEFORE_ACCEPTED) return "CANCELED_REVOKED_BEFORE_ACCEPTED";
if (e == IssueState::CANCELED_ISSUE_NOT_ACCEPTED) return "CANCELED_ISSUE_NOT_ACCEPTED";
if (e == IssueState::CANCELED_AFTER_REVOCATION_DURING_DISCUSSION) return "CANCELED_AFTER_REVOCATION_DURING_DISCUSSION";
if (e == IssueState::CANCELED_AFTER_REVOCATION_DURING_VERIFICATION) return "CANCELED_AFTER_REVOCATION_DURING_VERIFICATION";
if (e == IssueState::CANCELED_NO_INITIATIVE_ADMITTED) return "CANCELED_NO_INITIATIVE_ADMITTED";
if (e == IssueState::FINISHED_TABULATING_RESULTS) return "FINISHED_TABULATING_RESULTS";
if (e == IssueState::FINISHED_WITHOUT_WINNER) return "FINISHED_WITHOUT_WINNER";
if (e == IssueState::FINISHED_WITH_WINNER) return "FINISHED_WITH_WINNER";
    throw std::invalid_argument("unknown enum");
  }

}  