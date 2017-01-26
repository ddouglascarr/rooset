cmake_minimum_required(VERSION 3.2)
project(rooset-domain-command-posix)

# To run on ubuntu 16.04
# cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ../

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -Wall -pthread")
if(MSVC)
  # Havent run this on msvc for ages. May not work
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc /MTd")
endif(MSVC)
set(CMAKE_CXX_FLAGS "-std=c++14 -pthread -g -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/deps)


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


set(gtest_disable_pthreads OFF)
set(gmock_disable_pthreads OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/deps/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})


set(RATK_DECL_BASE_DIR ${PROJECT_SOURCE_DIR}/../rooset-declarations/src)
set(RATK_GEN_DIR ${PROJECT_SOURCE_DIR}/gen)
set(RATK_GEN_COMMANDS ON)
set(RATK_GEN_EVENTS ON)
set(RATK_GEN_ENUMS ON)
set(RATK_GEN_EXCEPTIONS ON)
set(RATK_GEN_TESTS ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/../rooset-application-toolkit/languages/cpp
    ${PROJECT_BINARY_DIR}/ratk)
include_directories(${RATK_INCLUDE_DIRS})


add_subdirectory(${PROJECT_SOURCE_DIR}/include)
add_library(librooset
    ${LIBROOSET_SOURCES})


add_executable(rooset-command
	${PROJECT_SOURCE_DIR}/main.cpp)
TARGET_LINK_LIBRARIES(rooset-command libratk librooset)
set_property(TARGET rooset-command PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-command PROPERTY CXX_STANDARD_REQUIRED ON)


enable_testing()
add_executable(rooset-generated-tests ${RATK_GEN_TEST_SOURCES})
target_link_libraries(rooset-generated-tests librooset libratk gmock gmock_main)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD_REQUIRED ON)
add_test(NAME generated COMMAND rooset-generated-tests)


add_subdirectory(${PROJECT_SOURCE_DIR}/tests/unit)
add_executable(rooset-unit-tests ${ROOSET_UNIT_TEST_SOURCES})
target_link_libraries(rooset-unit-tests librooset libratk gmock gmock_main)
set_property(TARGET rooset-unit-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
add_test(NAME unit COMMAND rooset-unit-tests)
