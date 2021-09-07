cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

add_library(variant19 INTERFACE)
file(
    GLOB_RECURSE
    co_variant_19_includes
    CONFIGURE_DEPENDS
    "*.h")

target_include_directories(variant19 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)
target_link_libraries(variant19 INTERFACE meta19)

if(${co-cpp19-enable-tests})
    file(
        GLOB_RECURSE
        variant19-tests
        "*.test.cpp")

    add_executable(variant19Test ${variant19-tests})
    target_link_libraries(variant19Test gtest_main variant19)
    add_test(variant19Test variant19Test)
endif()
