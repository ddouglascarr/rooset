add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/commands)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/enums)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/exceptions)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(RATK_GENERATED_SOURCES
  ${RATK_GEN_COMMAND_SOURCES}
  ${RATK_GEN_EVENT_SOURCES}
  ${RATK_GEN_EXCEPTION_SOURCES}
  ${RATK_GEN_ENUM_SOURCES}
  
  CACHE FILEPATH "All ratk generated cpp sources")


set(RATK_GENERATED_TESTS
  ${RATK_GEN_TEST_SOURCES}

  CACHE FILEPATH "All ratk generated cpp tests")
