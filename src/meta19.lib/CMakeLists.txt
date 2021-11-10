cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

add_library(meta19 INTERFACE)
set(CMAKE_CXX_STANDARD 20)

target_include_directories(meta19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(meta19 INTERFACE array19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        meta19-tests
        "*.test.cpp")

    add_executable(meta19Test ${meta19-tests})
    target_link_libraries(meta19Test gtest_main meta19)
    add_test(meta19Test meta19Test)
endif()
