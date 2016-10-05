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

using namespace rooset;

namespace rooset {

  template<typename MemberAggregateRepositoryImpl>
  class MemberCommandHandler
  {
  private:
    unique_ptr<AggregateRepository<MemberAggregate>> repository =
      make_unique<MemberAggregateRepositoryImpl>();

  public:
    unique_ptr<ProjectEvent> evaluate(const CreateAdminMemberCommand& c)
    {
      repository->assertAggregateDoesNotExist(c.id);
      return make_unique<AdminMemberCreatedEvent>(c);
    }

    unique_ptr<ProjectEvent> evaluate(const rooset::UpdateMemberPasswordCommand& c)
    {
      auto member = repository->load(c.id);
      if (member->getPassword() != c.oldPassword)
    }
  

  };

}

