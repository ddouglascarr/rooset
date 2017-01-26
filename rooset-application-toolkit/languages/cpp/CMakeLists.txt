cmake_minimum_required(VERSION 3.0)

# cmake input variables:
#  - RATK_DECL_BASE_DIR Location of the ratk declarations
#  - RATK_GEN_DIR Location that generated files should go (is added as include dir)
#  - RATK_GEN_COMMANDS Generate commands
#  - RATK_GEN_EVENTS Generate events
#  - RATK_GEN_ENUMS Generate enums
#  - RATK_GEN_EXCEPTIONS Generate exceptions
#  - RATK_GEN_TESTS Generate tests

# cmake output variables:
#  - RATK_INCLUDE_DIRS Add to the root include_directories

# cmake output libraries:
#  - libratk link to anything requiring headers from RATK_INCLUDE_DIRS


# Locally useful
set(RATK_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tools")

# Schema variables
set(RATK_GEN_BASE_SCHEMA_FILE "${RATK_DECL_BASE_DIR}/base.schema.json"
    CACHE FILEPATH "Location of ratk base schema.json file")

# Command variables
set(RATK_GEN_COMMAND_CPP_DEST_DIR "${RATK_GEN_DIR}/commands"
    CACHE PATH "Location of generated commands")
set(RATK_GEN_COMMAND_DECL_DIR "${RATK_DECL_BASE_DIR}/commands"
    CACHE PATH "Location of command ratk declarations")

# Event variables
set(RATK_GEN_EVENT_CPP_DEST_DIR "${RATK_GEN_DIR}/events"
    CACHE PATH "Location of generated events")
set(RATK_GEN_EVENT_DECL_DIR "${RATK_DECL_BASE_DIR}/events"
    CACHE PATH "Location of event ratk declarations")

# Enum variables
set(RATK_GEN_ENUM_CPP_DEST_DIR "${RATK_GEN_DIR}/enums"
    CACHE PATH "Location of generated enums")

# Exception variables
set(RATK_GEN_EXCEPTION_CPP_DEST_DIR "${RATK_GEN_DIR}/exceptions"
    CACHE PATH "Location of generated exceptions")
set(RATK_GEN_EXCEPTION_DECL_DIR "${RATK_DECL_BASE_DIR}/exceptions"
    CACHE PATH "Location of exception ratk declarations")

# Test variables
set(RATK_GEN_TEST_CPP_DEST_DIR "${RATK_GEN_DIR}/tests"
    CACHE PATH "Locations of test ratk declarations")
set(RATK_GEN_TEST_DECL_DIR "${RATK_DECL_BASE_DIR}/tests"
    CACHE PATH "Location of test ratk declarations")


# run npm install on cpp tools
message(STATUS "Installing node.js dependencies")
execute_process(
    COMMAND npm install
    WORKING_DIRECTORY ${RATK_TOOLS_DIR}
    RESULT_VARIABLE rv
    OUTPUT_VARIABLE exout)
if (rv STRGREATER 0)
  message(STATUS "npm install failed: ${rv}")
  message(STATUS ${exout})
else()
  message(STATUS ${exout})
endif()



# run npm install on ratk-declarations
message(STATUS "Installing node.js dependencies")
execute_process(
    COMMAND npm install
    WORKING_DIRECTORY ${RATK_TOOLS_DIR}/../../../ratk-declarations-utils
    RESULT_VARIABLE rv
    OUTPUT_VARIABLE exout)
if (rv STRGREATER 0)
  message(STATUS "npm install failed: ${rv}")
  message(STATUS ${exout})
else()
  message(STATUS ${exout})
endif()



set(SET_ENV ${CMAKE_COMMAND} -E
  env RATK_GEN_BASE_SCHEMA_FILE=${RATK_GEN_BASE_SCHEMA_FILE}
  env RATK_GEN_COMMAND_CPP_DEST_DIR=${RATK_GEN_COMMAND_CPP_DEST_DIR}
  env RATK_GEN_COMMAND_DECL_DIR=${RATK_GEN_COMMAND_DECL_DIR}
  env RATK_GEN_EVENT_CPP_DEST_DIR=${RATK_GEN_EVENT_CPP_DEST_DIR}
  env RATK_GEN_EVENT_DECL_DIR=${RATK_GEN_EVENT_DECL_DIR}
  env RATK_GEN_ENUM_CPP_DEST_DIR=${RATK_GEN_ENUM_CPP_DEST_DIR}
  env RATK_GEN_EXCEPTION_CPP_DEST_DIR=${RATK_GEN_EXCEPTION_CPP_DEST_DIR}
  env RATK_GEN_EXCEPTION_DECL_DIR=${RATK_GEN_EXCEPTION_DECL_DIR}
  env RATK_GEN_TEST_DECL_DIR=${RATK_GEN_TEST_DECL_DIR}
  env RATK_GEN_TEST_CPP_DEST_DIR=${RATK_GEN_TEST_CPP_DEST_DIR})

set(RUN_GEN_COMMANDS ${SET_ENV} ${RATK_TOOLS_DIR}/generate-cpp-commands)
set(RUN_GEN_EVENTS ${SET_ENV} ${RATK_TOOLS_DIR}/generate-cpp-events)
set(RUN_GEN_ENUMS ${SET_ENV} ${RATK_TOOLS_DIR}/generate-cpp-enums)
set(RUN_GEN_EXCEPTIONS ${SET_ENV} ${RATK_TOOLS_DIR}/generate-cpp-exceptions)
set(RUN_GEN_TESTS ${SET_ENV} ${RATK_TOOLS_DIR}/generate-cpp-tests)


if(RATK_GEN_COMMANDS)
  message(STATUS "ratk: generating commands")
  execute_process(
      COMMAND ${RUN_GEN_COMMANDS}
      RESULT_VARIABLE rv
      OUTPUT_VARIABLE exout)
  if (rv STRGREATER 0)
    message(STATUS " Command Generation Failed: ${rv}")
    message(STATUS ${exout})
  else()
    message(STATUS ${exout})
  endif()
  add_subdirectory(${RATK_GEN_COMMAND_CPP_DEST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gen/commands")

  add_custom_target(
      generate-commands
      COMMAND ${RUN_GEN_COMMANDS}
      VERBATIM)
endif()


if(RATK_GEN_EVENTS)
  message(STATUS "ratk: generating events")
  execute_process(
      COMMAND ${RUN_GEN_EVENTS}
      RESULT_VARIABLE rv
      OUTPUT_VARIABLE exout)
  if (rv STRGREATER 0)
    message(ERROR " Event Generation Failed: ${rv}")
  else()
    message(STATUS ${exout})
  endif()
  add_subdirectory(${RATK_GEN_EVENT_CPP_DEST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gen/events")

  add_custom_target(
      generate-events
      COMMAND ${RUN_GEN_EVENTS}
      VERBATIM)
endif()


if(RATK_GEN_ENUMS)
  message(STATUS "ratk: generating enums")
  execute_process(
      COMMAND ${RUN_GEN_ENUMS}
      RESULT_VARIABLE rv
      OUTPUT_VARIABLE exout)
  if (rv STRGREATER 0)
    message(ERROR " Event Generation Failed: ${rv}")
  else()
    message(STATUS ${exout})
  endif()
  add_subdirectory(${RATK_GEN_ENUM_CPP_DEST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gen/enums")

  add_custom_target(
      generate-enums
      COMMAND ${RUN_GEN_ENUMS}
      VERBATIM)
endif()


if(RATK_GEN_EXCEPTIONS)
  message(STATUS "ratk: generating exceptions")
  execute_process(
      COMMAND ${RUN_GEN_EXCEPTIONS}
      RESULT_VARIABLE rv
      OUTPUT_VARIABLE exout)
  if (rv STRGREATER 0)
    message(ERROR " Event Generation Failed: ${rv}")
  else()
    message(STATUS ${exout})
  endif()
  add_subdirectory(${RATK_GEN_EXCEPTION_CPP_DEST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gen/exceptions")

  add_custom_target(
      generate-exceptions
      COMMAND ${RUN_GEN_EXCEPTIONS}
      VERBATIM)
endif()


if(RATK_GEN_TESTS)
  message(STATUS "ratk: generating tests")
  execute_process(
      COMMAND ${RUN_GEN_TESTS}
      RESULT_VARIABLE rv
      OUTPUT_VARIABLE exout)
  if (rv STRGREATER 0)
    message(ERROR " Test Generation Failed: ${rv}")
  else()
    message(STATUS ${exout})
  endif()
  add_subdirectory(${RATK_GEN_TEST_CPP_DEST_DIR} "${CMAKE_CURRENT_BINARY_DIR}/gen/tests")

  add_custom_target(
      generate-tests
      COMMAND ${RUN_GEN_TESTS}
      VERBATIM)
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(CURL REQUIRED)

set(RATK_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RATK_GEN_DIR}
    ${CURL_INCLUDE_DIRS}
    CACHE PATH "Include dirs for RATK infrastructure, generated headers, and dependencies")
include_directories(${RATK_INCLUDE_DIRS})


set(RATK_SOURCE_FILES
    ${RATK_INFRASTRUCTURE_SOURCE_FILES}
    ${RATK_GEN_COMMAND_SOURCES}
    ${RATK_GEN_EVENT_SOURCES}
    ${RATK_GEN_ENUM_SOURCES}
    ${RATK_GEN_EXCEPTION_SOURCES}
    CACHE PATH "Source dirs for ratk inrastructure, generated and dependencies")


add_library(libratk ${RATK_SOURCE_FILES})
if(RATK_GEN_COMMANDS)
  add_dependencies(libratk generate-commands)
endif()
if(RATK_GEN_EVENTS)
  add_dependencies(libratk generate-events)
endif()
if(RATK_GEN_ENUMS)
  add_dependencies(libratk generate-enums)
endif()
if(RATK_GEN_EXCEPTIONS)
  add_dependencies(libratk generate-exceptions)
endif()


set(RATK_DEP_LIBRARIES ${CURL_LIBRARIES})


target_link_libraries(libratk ${RATK_DEP_LIBRARIES})

