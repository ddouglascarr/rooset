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
include_directories($ENV{ROOSET_BASE_DIR}/deps/rapidjson/include)


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


set(gtest_disable_pthreads OFF)
set(gmock_disable_pthreads OFF)
add_subdirectory($ENV{ROOSET_BASE_DIR}/deps/googletest
  ${PROJECT_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})


add_subdirectory($ENV{ROOSET_BASE_DIR}/rooset-application-toolkit/languages/cpp
    ${PROJECT_BINARY_DIR}/ratk)
add_subdirectory($ENV{ROOSET_BASE_DIR}/gen/cpp ${PROJECT_BINARY_DIR}/gen)
include_directories(${RATK_INCLUDE_DIRS})
include_directories($ENV{RATK_CPP_DIR})

add_subdirectory(${PROJECT_SOURCE_DIR}/include)
add_library(rooset-domain-command
    ${RATK_GENERATED_SOURCES}
    ${LIBROOSET_SOURCES})

add_executable(rooset-command
	${PROJECT_SOURCE_DIR}/main.cpp)
TARGET_LINK_LIBRARIES(rooset-command ratk rooset-domain-command)
set_property(TARGET rooset-command PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-command PROPERTY CXX_STANDARD_REQUIRED ON)


enable_testing()
add_executable(rooset-generated-tests ${RATK_GENERATED_TESTS})
target_link_libraries(rooset-generated-tests rooset-domain-command ratk gmock gmock_main)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD_REQUIRED ON)
add_test(NAME command-generated COMMAND rooset-generated-tests)


add_subdirectory(${PROJECT_SOURCE_DIR}/tests/unit)
add_executable(rooset-unit-tests ${ROOSET_UNIT_TEST_SOURCES})
target_link_libraries(rooset-unit-tests rooset-domain-command ratk gmock gmock_main)
set_property(TARGET rooset-unit-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-unit-tests PROPERTY CXX_STANDARD_REQUIRED ON)
add_test(NAME command-unit COMMAND rooset-unit-tests)
