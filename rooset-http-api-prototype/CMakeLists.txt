cmake_minimum_required(VERSION 3.2)
project(rooset-http-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )
set(CMAKE_CXX_FLAGS "-std=c++1y -pthread -g -Wall")

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  $ENV{ROOSET_BASE_DIR}/deps/rapidjson/include)
# r3.hpp uses angle brackets to import r3.h, needs to be top-level include
include_directories(/usr/include/r3)
add_subdirectory(include)

# librooset, libratk
include_directories($ENV{ROOSET_BASE_DIR}/rooset-domain-command-posix/gen)
add_subdirectory($ENV{ROOSET_BASE_DIR}/rooset-application-toolkit/languages/cpp 
  ${PROJECT_BINARY_DIR}/ratk)
include_directories(${RATK_INCLUDE_DIRS})

set(EXTERNAL_LIBS
  gflags
  glog
  wangle
  folly
  proxygenhttpserver
  r3
  ratk)

add_executable(rooset-http-server
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${ROOSET_HTTP_SERVER_SOURCES})

target_link_libraries(rooset-http-server
  ${EXTERNAL_LIBS})



################################################################################
#
# Testing
#
################################################################################

set(gtest_disable_pthreads OFF)
set(gmock_disable_pthreads OFF)
add_subdirectory($ENV{ROOSET_BASE_DIR}/deps/googletest
  ${PROJECT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/tests/unit)
add_executable(http-unit-tests 
  ${HTTP_UNIT_TEST_SOURCES}
  ${ROOSET_HTTP_SERVER_SOURCES})
target_link_libraries(http-unit-tests 
  ${EXTERNAL_LIBS}
  gmock
  gmock_main)
add_test(NAME unit COMMAND http-unit-tests)

add_subdirectory(${PROJECT_SOURCE_DIR}/tests/integration)
add_executable(http-integration-tests 
  ${HTTP_INTEGRATION_TEST_SOURCES}
  ${ROOSET_HTTP_SERVER_SOURCES})
target_link_libraries(http-integration-tests
  ${EXTERNAL_LIBS}
  gmock
  gmock_main)
add_test(NAME integration COMMAND http-integration-tests)
