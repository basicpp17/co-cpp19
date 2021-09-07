cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

add_library(signal19 INTERFACE)

target_include_directories(signal19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(signal19 INTERFACE array19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        signal19-tests
        "*.test.cpp")

    add_executable(signal19Test ${signal19-tests})
    target_link_libraries(signal19Test gtest_main signal19)
    add_test(signal19Test signal19Test)
endif()
