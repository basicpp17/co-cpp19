cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
add_library(optional19 INTERFACE)

target_include_directories(optional19
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(optional19 INTERFACE meta19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        optional19-tests
        "*.test.cpp")

    add_executable(optional19Test ${optional19-tests})
    target_link_libraries(optional19Test gtest_main optional19)
    add_test(optional19Test optional19Test)
endif()
