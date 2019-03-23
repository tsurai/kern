cmake_minimum_required(VERSION 3.0)
project(examples)

message(STATUS "Enabled examples")

set(EXAMPLE_NAMES
    minimal
    formatting
    filtering
    chaining
    advanced)

foreach(name ${EXAMPLE_NAMES})
    add_executable(${name} ${CMAKE_SOURCE_DIR}/examples/${name}.cpp)
    include_directories(${name} ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${name} kern)
endforeach()

