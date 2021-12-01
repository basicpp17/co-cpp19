cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

add_library(array19 INTERFACE)
set(CMAKE_CXX_STANDARD 20)

target_include_directories(array19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/> 
)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        array19-tests
        "*.test.cpp")

    add_executable(Array19Test ${array19-tests})
    target_link_libraries(Array19Test gtest_main array19)
    add_test(Array19Test Array19Test)
endif()

