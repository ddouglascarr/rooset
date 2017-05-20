cmake_minimum_required(VERSION 3.2)
project(rooset-http-server)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )
set(CMAKE_CXX_FLAGS "-std=c++1y -pthread -g -Wall")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(include)

add_executable(rooset-http-server
  ${PROJECT_SOURCE_DIR}/main.cpp
  ${ROOSET_HTTP_SERVER_SOURCES})

target_link_libraries(rooset-http-server
  gflags
  glog
  wangle
  folly
  proxygenhttpserver)


# Testing
set(gtest_disable_pthreads OFF)
set(gmock_disable_pthreads OFF)
add_subdirectory($ENV{ROOSET_BASE_DIR}/deps/googletest
  ${PROJECT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

enable_testing()

add_subdirectory(${PROJECT_SOURCE_DIR}/tests/unit)
add_executable(http-unit-tests ${HTTP_UNIT_TEST_SOURCES})
target_link_libraries(http-unit-tests 
  gflags
  glog
  wangle
  folly
  proxygenhttpserver
  gmock
  gmock_main)
add_test(NAME unit COMMAND http-unit-tests)
