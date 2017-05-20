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
