#include "IssueAggregate.h"



rooset::IssueAggregate::IssueAggregate(const NewInitiativeCreatedEvent& e) :
    id(e.id), unitId(e.unitId), areaId(e.areaId),
    policyId(e.policyId), issueState(IssueState::ADMISSION)
{
  initiatives[e.initiativeId] = Initiative{ e.name, e.requesterId, e.created };
}



void rooset::IssueAggregate::handleEvent(const CompetingInitiativeCreatedEvent & e)
{
  initiatives[e.initiativeId] = Initiative{ e.name, e.requesterId, e.created };
}



void rooset::IssueAggregate::handleEvent(const IssueDelegationSetEvent& e)
{
  delegations[e.trusterId] = e.trusteeId;
  // remove block if present
  auto i = find(blockedDelegations.begin(), blockedDelegations.end(), e.trusterId);
  if (i != blockedDelegations.end()) blockedDelegations.erase(i);
}



void rooset::IssueAggregate::handleEvent(const IssueDelegationUnsetEvent& e)
{
  delegations.erase(e.trusterId);
}



void rooset::IssueAggregate::handleEvent(const DelegationBlockedForIssueEvent& e)
{
  blockedDelegations.push_back(e.trusterId);
  // remove delegation if present
  auto i = delegations.find(e.trusterId);
  if (i != delegations.end()) delegations.erase(i);
}



void rooset::IssueAggregate::handleEvent(const DelegationUnblockedForIssueEvent& e)
{
  auto i = find(blockedDelegations.begin(), blockedDelegations.end(), e.trusterId);
  if (i != blockedDelegations.end()) blockedDelegations.erase(i);
}



void rooset::IssueAggregate::handleEvent(const InitiativeSupportGivenEvent& e)
{
  initiatives.at(e.initiativeId).supporters.push_back(e.requesterId);
}



void rooset::IssueAggregate::handleEvent(const InitiativeSupportRevokedEvent& e)
{
  auto& supporters = initiatives.at(e.initiativeId).supporters;
  auto i = find(supporters.begin(), supporters.end(), e.requesterId);
  if (i != supporters.end()) supporters.erase(i);
}



void rooset::IssueAggregate::handleEvent(const IssueAdmissionQuorumPassedEvent& e)
{
  issueState = IssueState::DISCUSSION;
}



void rooset::IssueAggregate::handleEvent(const IssueAdmissionQuorumFailedEvent& e)
{
  issueState = IssueState::CANCELED_NO_INITIATIVE_ADMITTED;
}



void rooset::IssueAggregate::handleEvent(const IssueDiscussionPhaseCompletedEvent& e)
{
  issueState = IssueState::VERIFICATION;
}



void rooset::IssueAggregate::handleEvent(const IssueVerificationPhaseCompletedEvent& e)
{
  issueState = IssueState::VOTING;
}



void rooset::IssueAggregate::handleEvent(const IssueVotingPhaseCompletedEvent& e)
{
  issueState = IssueState::FINISHED_WITH_WINNER;
}



void rooset::IssueAggregate::handleEvent(const IssueBallotSetEvent& e)
{
  ballots.emplace(e.requesterId, e.ballot);
}



void rooset::IssueAggregate::handleEvent(const IssueBallotUnsetEvent& e)
{
  auto it = ballots.find(e.requesterId);
  if (it != ballots.end()) ballots.erase(it);
}