#include "IssueCommandHandler.h"
#include <stdexcept>
#include <algorithm>
#include <memory>
#include "IssueCommandHandler.h"
#include "exceptions/CommandEvaluationException.h"
#include "PrivilegeUtils.h"
#include "CommandHandlerUtils.h"



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CreateNewInitiativeCommand& c)
{
  issueRepository->assertAggregateDoesNotExist(c.id);
  auto unit = unitRepository->load(c.unitId);
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  const Area area = CommandHandlerUtils::getActive<Area>(
      unit->getAreas(), c.areaId);
  const Concern concern = CommandHandlerUtils::getActive<Concern>(
      unit->getConcerns(), c.concernId);
  const Policy policy = CommandHandlerUtils::getActive<Policy>(
      unit->getPolicies(), c.policyId);
  CommandHandlerUtils::assertVectorContains<uuid>(
      area.concerns, c.concernId, "The concern is not of the area");
  CommandHandlerUtils::assertVectorContains<uuid>(
      concern.policies, c.policyId, "The policy is not of the concern");
  
  return make_unique<NewInitiativeCreatedEvent>(c);
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CreateCompetingInitiativeCommand & c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  assertIssueState(*issue,
      { IssueState::ADMISSION, IssueState::DISCUSSION, IssueState::VERIFICATION });
  PrivilegeUtils::assertInitiativeRight(*unit, c.requesterId);
  CommandHandlerUtils::assertMapExcludes<decltype(issue->getInitiatives()), uuid>(
      issue->getInitiatives(), c.initiativeId, "An initiative by that id already exists");
  return unique_ptr<CompetingInitiativeCreatedEvent>(
      new CompetingInitiativeCreatedEvent(c));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const SetIssueDelegationCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  PrivilegeUtils::assertVotingRight(*unit, c.requesterId);
  PrivilegeUtils::assertVotingRight(*unit, c.trusteeId);
  assertIssueState(*issue, 
      { IssueState::ADMISSION, IssueState::DISCUSSION, IssueState::VERIFICATION, IssueState::VOTING });
  return unique_ptr<IssueDelegationSetEvent>(new IssueDelegationSetEvent(
      c.id, c.requesterId, c.trusteeId));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const UnsetIssueDelegationCommand& c)
{
  auto issue = issueRepository->load(c.id);
  assertIssueState(*issue,
      { IssueState::ADMISSION, IssueState::DISCUSSION, IssueState::VERIFICATION, IssueState::VOTING });
  CommandHandlerUtils::assertMapContains<decltype(issue->getDelegations()), uuid>(
      issue->getDelegations(), c.requesterId, "No delegation is set for this issue");
  return unique_ptr<IssueDelegationUnsetEvent>(new IssueDelegationUnsetEvent(
      c.id, c.requesterId));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const BlockDelegationForIssueCommand& c)
{
  const auto trusterId = c.requesterId;
  const auto issue = issueRepository->load(c.id);
  const auto unit = unitRepository->load(issue->getUnitId());

  assertIssueState(*issue, {
    IssueState::ADMISSION,
    IssueState::DISCUSSION,
    IssueState::VERIFICATION,
    IssueState::VOTING,
  });
  PrivilegeUtils::assertVotingRight(*unit, trusterId);
  CommandHandlerUtils::assertVectorExcludes<uuid>(issue->getBlockedDelegations(), trusterId,
      "Delegations are already blocked for this issue");
  return unique_ptr<DelegationBlockedForIssueEvent>(new DelegationBlockedForIssueEvent(
      c.id, trusterId));
 }



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const UnblockDelegationForIssueCommand& c)
{
  const auto trusterId = c.requesterId;
  const auto issue = issueRepository->load(c.id);
  assertIssueState(*issue, {
    IssueState::ADMISSION,
    IssueState::DISCUSSION,
    IssueState::VERIFICATION,
    IssueState::VOTING,
  });
  CommandHandlerUtils::assertVectorContains<uuid>(issue->getBlockedDelegations(), trusterId,
      "Delegations are not blocked for this issue");
  return unique_ptr<DelegationUnblockedForIssueEvent>(new DelegationUnblockedForIssueEvent(
      c.id, trusterId));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const GiveInitiativeSupportCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  assertIssueState(*issue, { IssueState::ADMISSION, IssueState::DISCUSSION, IssueState::VERIFICATION });
  PrivilegeUtils::assertVotingRight(*unit, c.requesterId);
  CommandHandlerUtils::assertMapContains<decltype(issue->getInitiatives()), uuid>(
      issue->getInitiatives(), c.initiativeId, "No initiative for that initiativeId");
  CommandHandlerUtils::assertVectorExcludes<uuid>(
      issue->getInitiatives().at(c.initiativeId).supporters,
      c.requesterId, "Member already supports this initiative");
  return unique_ptr<InitiativeSupportGivenEvent>(new InitiativeSupportGivenEvent(c));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const RevokeInitiativeSupportCommand& c)
{
  auto issue = issueRepository->load(c.id);
  assertIssueState(*issue, { IssueState::ADMISSION, IssueState::DISCUSSION, IssueState::VERIFICATION });
  CommandHandlerUtils::assertMapContains<decltype(issue->getInitiatives()), uuid>(
      issue->getInitiatives(), c.initiativeId, "No initiative for that initiativeId");
  CommandHandlerUtils::assertVectorContains<uuid>(
      issue->getInitiatives().at(c.initiativeId).supporters,
      c.requesterId, "Member does not support this initiative");
  return unique_ptr<InitiativeSupportRevokedEvent>(new InitiativeSupportRevokedEvent(c));
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const AssessIssueAdmissionQuorumCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  if (isAdmissionQuorumPassed(*issue, *unit)) {
    return make_unique<IssueAdmissionQuorumPassedEvent>(c.id);
  }
  return make_unique<IssueAdmissionQuorumContinuedEvent>(c.id);
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CompleteIssueAdmissionPhaseCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  if (isAdmissionQuorumPassed(*issue, *unit)) {
    return make_unique<IssueAdmissionQuorumPassedEvent>(c.id);
  }
  return make_unique<IssueAdmissionQuorumFailedEvent>(c.id);
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CompleteIssueDiscussionPhaseCommand& c)
{
  return nullptr;
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CompleteIssueVerificationPhaseCommand& c)
{
  return nullptr;
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const SetIssueBallotCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());
  assertIssueState(*issue, { IssueState::VOTING });
  PrivilegeUtils::assertVotingRight(*unit, c.requesterId);
  return make_unique<IssueBallotSetEvent>(c);
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const UnsetIssueBallotCommand& c)
{
  auto issue = issueRepository->load(c.id);
  CommandHandlerUtils::assertMapContains<map<uuid, SchulzeBallot>, uuid>(
      issue->getBallots(), c.requesterId, "Member has not set a ballot");
  return make_unique<IssueBallotUnsetEvent>(c);
}



unique_ptr<ProjectEvent<Document>> rooset::IssueCommandHandler::evaluate(
    const CompleteIssueVotingPhaseCommand& c)
{
  auto issue = issueRepository->load(c.id);
  auto unit = unitRepository->load(issue->getUnitId());

  const auto& ballotsMap = issue->getBallots();
  const auto& initiatives = issue->getInitiatives();
  const auto& privileges = unit->getPrivileges();
  const auto delegations = delegationCalculator->calcIssueDelegations(
    *issue,
    unit->getAreas().at(issue->getAreaId()),
    *unit,
    privileges);

  // set ballot weights
  vector<SchulzeBallot> ballots;
  for (auto it = ballotsMap.begin(); it != ballotsMap.end(); ++it) {
    const uuid voterId = it->first;
    auto ballot = it->second;
    ballot.setWeight(delegationCalculator->calcMemberWeight(
        privileges, delegations, voterId));
    ballots.push_back(ballot);
  }
  auto winners = voteCalculator->calcWinners(ballots, initiatives);

  // tie breaker. Pick status quo or the earliest created initiative
  uuid winnerId = idTools->generateNilId();
  bool winnerIdSet = false;
  for (auto& w : winners) {
    if (!winnerIdSet) {
      winnerIdSet = true;
      winnerId = w;
    } else if (
        winnerId != idTools->generateNilId() &&
        initiatives.at(w).created < initiatives.at(winnerId).created) {
      winnerId = w;
    }
  }
  return make_unique<IssueVotingPhaseCompletedEvent>(c.id, winnerId);
}



void rooset::IssueCommandHandler::assertIssueState(
    const IssueAggregate& issue,
    const vector<IssueState>& acceptable)
{
  auto i = find(acceptable.begin(), acceptable.end(), issue.getIssueState());
  if (i == acceptable.end()) {
    throw CommandEvaluationException(ExceptionCode::ISSUE_STATE_EXCEPTION,
        "Issue is in the wrong state for this command");
  }
}



bool rooset::IssueCommandHandler::isAdmissionQuorumPassed(
    const IssueAggregate& issue, const UnitAggregate& unit)
{
  assertIssueState(issue, { IssueState::ADMISSION });
  auto area = unit.getAreas().at(issue.getAreaId());
  const auto totalVoteWeight = calcTotalVoteWeight(unit.getPrivileges());
  const auto policy = unit.getPolicies().at(issue.getPolicyId());
  const auto initiatives = issue.getInitiatives();
  for (auto it : initiatives) {
    const auto initiative = it.second;
    auto support = 0;
    const auto& privileges = unit.getPrivileges();
    const auto& delegations = delegationCalculator->calcInitiativeDelegations(
        initiative, issue, area, unit, privileges);
    for (auto supporterId : initiative.supporters) {
      support += delegationCalculator->calcMemberWeight(privileges, delegations, supporterId);
    }
    if (isAdmissionQuorumPassed(policy, totalVoteWeight, support)) {
      return true;
    }
  }
  return false;
}



bool rooset::IssueCommandHandler::isAdmissionQuorumPassed(
    const rooset::Policy& policy, 
    const unsigned long long totalVoteWeight,
    const unsigned long long support)
{
  return (support * policy.issueQuorumDen) >= (policy.issueQuorumNum * totalVoteWeight);
}



unsigned long long rooset::IssueCommandHandler::calcTotalVoteWeight(
    const map<uuid, rooset::MemberPrivilege>& privileges)
{
  auto totalVoteWeight = 0;
  for (auto it : privileges) {
    const auto privilege = it.second;
    if (privilege.votingRight) {
      totalVoteWeight += privilege.weight;
    }
  }
  return totalVoteWeight;
}



