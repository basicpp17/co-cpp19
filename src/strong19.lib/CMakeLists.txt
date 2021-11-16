cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

set(CMAKE_CXX_STANDARD 20)
add_library(strong19 INTERFACE)

target_include_directories(strong19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)


add_subdirectory(../meta19.lib metaBuild)
add_subdirectory(../string19.lib stringBuild)

target_link_libraries(strong19 INTERFACE meta19 string19)

if(${co-cpp19-enable-tests})
    add_subdirectory(../../third_party build)
    enable_testing()
    file(
        GLOB_RECURSE
        strong19-tests
        "*.test.cpp")

    add_executable(strong19Test ${strong19-tests})
    target_link_libraries(strong19Test gtest_main strong19)
    add_test(strong19Test strong19Test)
endif()
