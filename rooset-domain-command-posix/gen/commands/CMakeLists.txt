

set(RATK_GEN_COMMAND_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/AddAreaConcernCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AddPolicyCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AssessIssueAdmissionQuorumCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/BlockDelegationForAreaCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/BlockDelegationForIssueCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CompleteIssueAdmissionPhaseCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CompleteIssueDiscussionPhaseCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CompleteIssueVerificationPhaseCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CompleteIssueVotingPhaseCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CreateAreaCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CreateCompetingInitiativeCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CreateConcernCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CreateNewInitiativeCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/CreateUnitCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DeactivateConcernCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/GiveInitiativeSupportCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/GrantPrivilegeCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/RemoveAreaConcernCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/RevokeInitiativeSupportCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SetAreaDelegationCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SetIssueBallotCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SetIssueDelegationCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SetUnitDelegationCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnblockDelegationForAreaCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnblockDelegationForIssueCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnsetAreaDelegationCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnsetIssueBallotCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnsetIssueDelegationCommand.cpp
${CMAKE_CURRENT_SOURCE_DIR}/UnsetUnitDelegationCommand.cpp

  CACHE FILEPATH "Generated source files")

