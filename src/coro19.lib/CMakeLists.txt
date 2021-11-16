cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
add_library(coro19 STATIC)

target_include_directories(coro19 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

target_sources(coro19 PRIVATE
    coro19/coroutine.cpp
)

if(${co-cpp19-enable-tests})
    add_subdirectory(../../third_party build)
    enable_testing()
    file(
        GLOB_RECURSE
        coro19-tests
        "*.test.cpp")

    add_executable(Coro19Test ${coro19-tests})
    target_link_libraries(Coro19Test gtest_main coro19)
    add_test(Coro19Test Coro19Test)
endif()
