#pragma once
#include <memory>
#include "framework/uuid.h"
#include "framework/AggregateRepository.h"
#include "framework/ProjectEvent.h"
#include "MemberAggregate.h"
#include "commands/CreateAdminMemberCommand.h"
#include "commands/UpdateMemberPasswordCommand.h"
#include "commands/CreateUnitCommand.h"
#include "events/AdminMemberCreatedEvent.h"
#include "events/MemberPasswordUpdatedEvent.h"
#include "events/UnitCreatedEvent.h"
#include "exceptions/CommandEvaluationException.h"
#include "enums/ExceptionCode.h"

using namespace rooset;

namespace rooset {

  class MemberCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<MemberAggregate>> repository;

  public:
    MemberCommandHandler(unique_ptr<AggregateRepository<MemberAggregate>> r) : repository(move(r))
    { }

    auto evaluate(const CreateAdminMemberCommand& c)
    {
      repository->assertAggregateDoesNotExist(c.id);
      return make_unique<AdminMemberCreatedEvent>(
          c.id, c.login, c.password, c.name, c.notifyEmail);
    }

    auto evaluate(const rooset::UpdateMemberPasswordCommand& c)
    {
      auto member = repository->load(c.id);
      if (member->getPassword() != c.oldPassword) {
        throw CommandEvaluationException(ExceptionCode::CONFLICT_EXCEPTION, 
            "The old password does not match");
      }
      if (c.id != c.requesterId) {
        throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
            "A member may only change their own password");
      }
      return unique_ptr<MemberPasswordUpdatedEvent>(new MemberPasswordUpdatedEvent{
          c.id, c.requesterId, c.newPassword });
    }

    auto evaluate(const rooset::CreateUnitCommand& c)
    {
      auto requestingMember = repository->load(c.requesterId);
      if (!requestingMember->getIsAdmin()) {
        throw CommandEvaluationException(ExceptionCode::UNPRIVILEGED_EXCEPTION,
            "Only admin users can create units");
      }
      return unique_ptr<UnitCreatedEvent>(new UnitCreatedEvent(
          c.id, c.requesterId, c.name, c.description));
    }
  };

}

