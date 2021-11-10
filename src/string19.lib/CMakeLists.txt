cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
add_library(string19 INTERFACE)

target_include_directories(string19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(string19 INTERFACE meta19 tuple19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        string19-tests
        "*.test.cpp")

    add_executable(string19Test ${string19-tests})
    target_link_libraries(string19Test gtest_main string19)
    add_test(string19Test string19Test)
endif()
