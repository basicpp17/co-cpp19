cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

set(CMAKE_CXX_STANDARD 20)
add_library(tuple19 INTERFACE)

target_include_directories(tuple19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)


add_subdirectory(../meta19.lib metaBuild)

target_link_libraries(tuple19 INTERFACE meta19)

if(${co-cpp19-enable-tests})
    add_subdirectory(../../third_party build)
    enable_testing()
    file(
        GLOB_RECURSE
        tuple19-tests
        "*.test.cpp")

    add_executable(tuple19Test ${tuple19-tests})
    target_link_libraries(tuple19Test gtest_main tuple19)
    add_test(tuple19Test tuple19Test)
endif()
