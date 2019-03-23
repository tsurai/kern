cmake_minimum_required(VERSION 3.0)

message(STATUS "Enabled testing")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/tests/catch)
include_directories(${CMAKE_SOURCE_DIR}/tests/catch ${COMMON_INCLUDES})

set(TEST_SOURCES
    testbase.cpp
    testbuilder.cpp
    testdispatch.cpp
    testsink.cpp
    testmacros.cpp
    testoperators.cpp
    utils.cpp)

add_executable(tests
    ${TEST_SOURCES})

include(ParseAndAddCatchTests)
set(PARSE_CATCH_TESTS_ADD_TARGET_IN_TEST_NAME OFF)
ParseAndAddCatchTests(tests)

include_directories(tests ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests kern)

enable_testing()
