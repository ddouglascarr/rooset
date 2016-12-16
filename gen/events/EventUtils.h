
#pragma once
#include <functional>
#include "exceptions/CommandEvaluationException.h"
#include "rapidjson/document.h"
#include "framework/MessageUtils.h"
#include "framework/JsonUtils.h"

#include "events/AdminMemberCreatedEvent.h"
#include "events/AreaCreatedEvent.h"
#include "events/MemberCreatedEvent.h"
#include "events/MemberDetailsUpdatedEvent.h"
#include "events/MemberPasswordUpdatedEvent.h"
#include "events/PrivilegeGrantedEvent.h"
#include "events/UnitCreatedEvent.h"
#include "events/AreaDelegationSetEvent.h"
#include "events/AreaDelegationUnsetEvent.h"
#include "events/CompetingInitiativeCreatedEvent.h"
#include "events/NewInitiativeCreatedEvent.h"
#include "events/UnitDelegationSetEvent.h"
#include "events/UnitDelegationUnsetEvent.h"
#include "events/UnitPolicySetEvent.h"

namespace rooset {

  class EventUtils {
  public:
    template<typename Aggregate>
    static void applyEvent(
        unique_ptr<Aggregate>& aggregate,
        const rapidjson::Document& d,
        std::function<void()> onMethodMissing)
    {
      const string msgType = d["type"].GetString();

      
      if (msgType == "ADMIN_MEMBER_CREATED_EVENT") {
        JsonUtils::validate(*AdminMemberCreatedEvent::schema, d);
        const AdminMemberCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AdminMemberCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_CREATED_EVENT") {
        JsonUtils::validate(*AreaCreatedEvent::schema, d);
        const AreaCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "MEMBER_CREATED_EVENT") {
        JsonUtils::validate(*MemberCreatedEvent::schema, d);
        const MemberCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, MemberCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "MEMBER_DETAILS_UPDATED_EVENT") {
        JsonUtils::validate(*MemberDetailsUpdatedEvent::schema, d);
        const MemberDetailsUpdatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, MemberDetailsUpdatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "MEMBER_PASSWORD_UPDATED_EVENT") {
        JsonUtils::validate(*MemberPasswordUpdatedEvent::schema, d);
        const MemberPasswordUpdatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, MemberPasswordUpdatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "PRIVILEGE_GRANTED_EVENT") {
        JsonUtils::validate(*PrivilegeGrantedEvent::schema, d);
        const PrivilegeGrantedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, PrivilegeGrantedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_CREATED_EVENT") {
        JsonUtils::validate(*UnitCreatedEvent::schema, d);
        const UnitCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_DELEGATION_SET_EVENT") {
        JsonUtils::validate(*AreaDelegationSetEvent::schema, d);
        const AreaDelegationSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaDelegationSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "AREA_DELEGATION_UNSET_EVENT") {
        JsonUtils::validate(*AreaDelegationUnsetEvent::schema, d);
        const AreaDelegationUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, AreaDelegationUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "COMPETING_INITIATIVE_CREATED_EVENT") {
        JsonUtils::validate(*CompetingInitiativeCreatedEvent::schema, d);
        const CompetingInitiativeCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, CompetingInitiativeCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "NEW_INITIATIVE_CREATED_EVENT") {
        JsonUtils::validate(*NewInitiativeCreatedEvent::schema, d);
        const NewInitiativeCreatedEvent evt(d);
        MessageUtils::applyEvent<Aggregate, NewInitiativeCreatedEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_DELEGATION_SET_EVENT") {
        JsonUtils::validate(*UnitDelegationSetEvent::schema, d);
        const UnitDelegationSetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitDelegationSetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_DELEGATION_UNSET_EVENT") {
        JsonUtils::validate(*UnitDelegationUnsetEvent::schema, d);
        const UnitDelegationUnsetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitDelegationUnsetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      if (msgType == "UNIT_POLICY_SET_EVENT") {
        JsonUtils::validate(*UnitPolicySetEvent::schema, d);
        const UnitPolicySetEvent evt(d);
        MessageUtils::applyEvent<Aggregate, UnitPolicySetEvent>(aggregate, evt, onMethodMissing);
        return;
      }

      throw CommandEvaluationException(
          ExceptionCode::GENERAL_PROJECT_EXCEPTION,
          "There is no ifBlock for " + msgType + ". Run make again, and check the generateEventUtils script");
    }

  };
}


  