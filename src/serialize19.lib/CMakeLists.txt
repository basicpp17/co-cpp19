cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

set(CMAKE_CXX_STANDARD 20)
add_library(serialize19 INTERFACE)

target_include_directories(serialize19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

add_subdirectory(../array19.lib metaBuild)
add_subdirectory(../optional19.lib optional19)
add_subdirectory(../tuple19.lib tuple19)
add_subdirectory(../partial19.lib partial19)
add_subdirectory(../variant19.lib variant19)

target_link_libraries(serialize19 INTERFACE tuple19 array19 partial19 variant19 optional19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        serialize19-tests
        "*.test.cpp")

    add_executable(serialize19Test ${serialize19-tests})
    target_link_libraries(serialize19Test gtest_main serialize19)
    add_test(serialize19Test serialize19Test)
endif()
