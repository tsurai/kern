cmake_minimum_required(VERSION 3.0)
project(kern)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)
set(PROJECT_VERSION 0.1.0)
set(LOGGER_SOURCES
    src/dispatch.cpp
    src/builder.cpp
    src/sink.cpp
    src/info.cpp)

add_library(kern SHARED ${LOGGER_SOURCES})
set_target_properties(kern PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(kern PROPERTIES PUBLIC_HEADER src/kern.h)

include(GNUInstallDirs)
install(TARGETS kern
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

option(TESTING "Enable unit tests with catch" OFF)
if(TESTING)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/tests/catch)
    include_directories(${CMAKE_SOURCE_DIR}/tests/catch ${COMMON_INCLUDES})

    set(TEST_SOURCES
        tests/testbase.cpp
        tests/testbuilder.cpp
        tests/testdispatch.cpp
        tests/testsink.cpp
        tests/testmacros.cpp
        tests/utils.cpp)

    add_executable(tests
        ${TEST_SOURCES})

    include(ParseAndAddCatchTests)
    set(PARSE_CATCH_TESTS_ADD_TARGET_IN_TEST_NAME OFF)
    ParseAndAddCatchTests(tests)

    include_directories(tests ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(tests kern)

    enable_testing()
endif()
