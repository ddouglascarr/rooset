cmake_minimum_required (VERSION 3.0)
project (rooset-domain)

# To run on ubuntu 16.04
# cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ ../

SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -Wall -pthread")
IF(MSVC)
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc /MTd")
ENDIF(MSVC)
set(CMAKE_CXX_FLAGS "-g -Wall") 

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 )
include_directories(${PROJECT_SOURCE_DIR}/rooset-domain)
include_directories(${PROJECT_SOURCE_DIR}/deps)
include_directories(${PROJECT_SOURCE_DIR}/gen)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.58 REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
# link_directories(${Boost_LIBRARY_DIR})
# add_definitions("-DBOOST_COROUTINES_NO_DEPRECATION_WARNING")

find_package(CURL)
MESSAGE(STATUS "CURL found: ${CURL_FOUND} ${CURL_VERSION_STRING}")
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})

FILE(GLOB project_h rooset-domain/**/*.h)
FILE(GLOB project_cpp rooset-domain/**/*.cpp)
SOURCE_GROUP(include FILES ${project_h} ${project_cpp})


FILE(GLOB generated_h gen/**/*.h)
SOURCE_GROUP(generated FILES ${generated_h})

FILE(GLOB generated_cpp gen/sources/*.cpp)
SOURCE_GROUP(generated FILES ${generated_cpp})

ADD_CUSTOM_TARGET(
	generate-sources
	COMMAND ${PROJECT_SOURCE_DIR}/tools/generate.sh
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	COMMENT run header and source generation
	VERBATIM)

ADD_LIBRARY(rooset-lib
	${generated_cpp}
	${project_cpp})
ADD_DEPENDENCIES(rooset-lib generate-sources)

add_executable(rooset-domain-server 
	${project_h}
	${generated_h}
	${PROJECT_SOURCE_DIR}/rooset-domain/main.cpp)
TARGET_LINK_LIBRARIES(rooset-domain-server ${CURL_LIBRARIES} rooset-lib)
set_property(TARGET rooset-domain-server PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-domain-server PROPERTY CXX_STANDARD_REQUIRED ON)


# Testing
set(gtest_disable_pthreads OFF)
SET(gmock_disable_pthreads OFF)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/deps/googletest)
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
INCLUDE_DIRECTORIES(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
MESSAGE(STATUS "** gmock source: ${gmock_SOURCE_DIR}")
ENABLE_TESTING()

FILE(GLOB generated_tests ${PROJECT_SOURCE_DIR}/gen/tests/*.cpp)
ADD_EXECUTABLE(rooset-generated-tests ${generated_tests})
TARGET_LINK_LIBRARIES(rooset-generated-tests rooset-lib gmock gmock_main)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-generated-tests PROPERTY CXX_STANDARD_REQUIRED ON)
ADD_TEST(NAME generated COMMAND rooset-generated-tests)

FILE(GLOB framework_tests ${PROJECT_SOURCE_DIR}/tests/*.cpp)
ADD_EXECUTABLE(rooset-framework-tests ${framework_tests})
TARGET_LINK_LIBRARIES(rooset-framework-tests rooset-lib gmock gmock_main)
set_property(TARGET rooset-framework-tests PROPERTY CXX_STANDARD 14)
set_property(TARGET rooset-framework-tests PROPERTY CXX_STANDARD_REQUIRED ON)
ADD_TEST(NAME framework COMMAND rooset-framework-tests)
