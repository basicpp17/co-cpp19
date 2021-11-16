cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

add_library(lookup19 INTERFACE)
set(CMAKE_CXX_STANDARD 20)

target_include_directories(lookup19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

add_subdirectory(../array19.lib arrayBuild)
target_link_libraries(lookup19 INTERFACE array19)

if(${co-cpp19-enable-tests})
    add_subdirectory(../../third_party build)
    enable_testing()
    file(
        GLOB_RECURSE
        lookup19-tests
        "*.test.cpp")

    add_executable(lookup19Test ${lookup19-tests})
    target_link_libraries(lookup19Test gtest_main lookup19)
    add_test(lookup19Test lookup19Test)
endif()
