#pragma once
#include <memory>
#include "../../framework/uuid.h"
#include "../../framework/AggregateRepository.h"
#include "../../framework/ProjectEvent.h"
#include "MemberAggregate.h"
#include "commands/CreateAdminMemberCommand.h"
#include "commands/UpdateMemberPasswordCommand.h"
#include "events/AdminMemberCreatedEvent.h"
#include "events/MemberPasswordUpdatedEvent.h"
#include "../../exceptions/ConflictException.h"
#include "../../exceptions/UnauthorizedException.h"

using namespace rooset;

namespace rooset {

  template<typename MemberAggregateRepositoryImpl>
  class MemberCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<MemberAggregate>> repository =
      make_unique<MemberAggregateRepositoryImpl>();

  public:
    auto evaluate(const CreateAdminMemberCommand& c)
    {
      repository->assertAggregateDoesNotExist(c.id);
      return make_unique<AdminMemberCreatedEvent>(c);
    }

    auto evaluate(const rooset::UpdateMemberPasswordCommand& c)
    {
      auto member = repository->load(c.id);
      if (member->getPassword() != c.oldPassword) {
        throw ConflictException("The old password does not match");
      }
      if (c.id != c.requesterId) {
        throw UnauthorizedException("A member may only change their own password");
      }
      return make_unique<MemberPasswordUpdatedEvent>(c);
    }
  };

}

