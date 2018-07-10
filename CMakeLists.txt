cmake_minimum_required(VERSION 3.0)
project(kern)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -D_FORTIFY_SOURCE=2")
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
elseif(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_VERSION 0.2.0)
set(KERN_SOURCES
    src/dispatch.cpp
    src/builder.cpp
    src/sink.cpp
    src/info.cpp)
set(KERN_HEADER
    src/kern.h
    src/dispatch.h
    src/builder.h
    src/info.h
    src/sink.h)

add_library(kern SHARED ${KERN_SOURCES})
set_target_properties(kern PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(kern PROPERTIES PUBLIC_HEADER "${KERN_HEADER}")

if(UNIX)
    include(GNUInstallDirs)
    install(TARGETS kern
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}" COMPONENT dev)

    set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkconfig files")
    configure_file("kern.pc.in" "kern.pc" @ONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/kern.pc" DESTINATION "${INSTALL_PKGCONFIG_DIR}")
elseif(WIN32)
    set_target_properties(kern PROPERTIES DEFINE_SYMBOL BUILD_DLL)
    install(TARGETS kern
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include COMPONENT devel)
endif()

option(TESTING "Enable unit tests with catch" OFF)
if(TESTING)
    if(DEFINED ENV{CODECOV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
        set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
        include(CodeCoverage)
        setup_target_for_coverage(${PROJECT_NAME}_coverage tests coverage)
    endif()

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
