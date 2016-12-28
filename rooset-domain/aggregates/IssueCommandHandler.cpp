#include "IssueCommandHandler.h"
#include <stdexcept>
#include <algorithm>
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
  CommandHandlerUtils::assertMapContains<decltype(unit->getAreas()), uuid>(
      unit->getAreas(), c.areaId, "Unit does not contain this area");
  return unique_ptr<NewInitiativeCreatedEvent>(new NewInitiativeCreatedEvent(c));
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
    for_each(initiative.supporters.begin(), initiative.supporters.end(),
        [&](uuid supporterId) { support = support + memberWeightCalculator->calcSupportWeight(
            supporterId, initiative, issue, area, unit); });
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



