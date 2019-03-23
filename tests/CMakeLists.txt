cmake_minimum_required(VERSION 3.0)

message(STATUS "Enabled testing")

if(DEFINED ENV{CODECOV})
    message(STATUS "Enabled codecov")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage tests coverage)
endif()

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
