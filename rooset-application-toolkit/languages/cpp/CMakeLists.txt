cmake_minimum_required(VERSION 3.0)

# cmake output libraries:
#  - RATK_INCLUDE_DIRS

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(CURL REQUIRED)

set(RATK_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $ENV{RATK_CPP_DIR}
    ${CURL_INCLUDE_DIRS}
    CACHE PATH "Include dirs for RATK infrastructure, generated headers, and dependencies")
include_directories(${RATK_INCLUDE_DIRS})


set(RATK_SOURCE_FILES
    ${RATK_INFRASTRUCTURE_SOURCE_FILES}
    CACHE PATH "Source dirs for ratk inrastructure, generated and dependencies")

add_library(ratk ${RATK_SOURCE_FILES})
set(RATK_DEP_LIBRARIES ${CURL_LIBRARIES})
target_link_libraries(ratk ${RATK_DEP_LIBRARIES})

