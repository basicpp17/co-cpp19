cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

add_library(partial19 INTERFACE)

target_include_directories(partial19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(partial19 INTERFACE array19 meta19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        partial19-tests
        "*.test.cpp")

    add_executable(partial19Test ${partial19-tests})
    target_link_libraries(partial19Test gtest_main partial19)
    add_test(partial19Test partial19Test)
endif()
