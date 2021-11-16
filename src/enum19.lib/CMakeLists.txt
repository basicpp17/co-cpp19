cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
include_guard(GLOBAL)

add_library(enum19 INTERFACE)
set(CMAKE_CXX_STANDARD 20)

target_include_directories(enum19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
add_subdirectory(../array19.lib arrayBuild)
add_subdirectory(../meta19.lib metaBuild)
add_subdirectory(../string19.lib stringBuild)
target_link_libraries(enum19 INTERFACE array19 meta19 string19)

if(${co-cpp19-enable-tests})
    add_subdirectory(../../third_party build)
    enable_testing()
    file(
        GLOB_RECURSE
        enum19-tests
        "*.test.cpp")

    add_executable(Enum19Test ${enum19-tests})
    target_link_libraries(Enum19Test gtest_main enum19)
    add_test(Enum19Test Enum19Test)
endif()
