cmake_minimum_required (VERSION 3.0)

project (rooset-domain)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )

include_directories(${PROJECT_SOURCE_DIR}/rooset-domain)
include_directories(${PROJECT_SOURCE_DIR}/deps)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

add_executable(rooset-domain-server ${PROJECT_SOURCE_DIR}/rooset-domain/main.cpp)
set_property(TARGET rooset-domain-server PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-domain-server PROPERTY CXX_STANDARD_REQUIRED ON)

set(TESTS_SRC_DIR ${PROJECT_SOURCE_DIR}/tests)
add_executable(rooset-domain-tests
  ${TESTS_SRC_DIR}/MemberAggregateTests.cpp
  ${TESTS_SRC_DIR}/testRunner.cpp)
set_property(TARGET rooset-domain-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-domain-tests PROPERTY CXX_STANDARD_REQUIRED ON)
