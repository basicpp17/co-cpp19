cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

add_library(serialize19 INTERFACE)

target_include_directories(serialize19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
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
