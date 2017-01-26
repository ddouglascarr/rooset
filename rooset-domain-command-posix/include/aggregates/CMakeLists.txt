set(ROOSET_AGGREGATE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/DelegationCalculatorImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/IssueCommandHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/IssueAggregate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MemberCommandHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/PrivilegeUtils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SchulzeBallot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UnitAggregate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UnitCommandHandler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/VoteCalculatorSchulzeImpl.cpp

  CACHE FILEPATH "Sources for the aggregtates")
