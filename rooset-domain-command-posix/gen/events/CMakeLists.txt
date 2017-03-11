

set(RATK_GEN_EVENT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AreaConcernAddedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AreaConcernRemovedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AreaCreatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AreaDelegationSetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AreaDelegationUnsetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CompetingInitiativeCreatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ConcernCreatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ConcernDeactivatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DelegationBlockedForAreaEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DelegationBlockedForIssueEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DelegationUnblockedForAreaEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DelegationUnblockedForIssueEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/InitiativeSupportGivenEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/InitiativeSupportRevokedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueAdmissionQuorumContinuedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueAdmissionQuorumFailedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueAdmissionQuorumPassedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueBallotSetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueBallotUnsetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueDelegationSetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueDelegationUnsetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueDiscussionPhaseCompletedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueVerificationPhaseCompletedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/IssueVotingPhaseCompletedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/NewInitiativeCreatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/PolicyAddedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/PrivilegeGrantedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnitCreatedEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnitDelegationSetEvent.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnitDelegationUnsetEvent.cpp

  CACHE FILEPATH "Generated source files")

