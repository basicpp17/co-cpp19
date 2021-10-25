cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project (co-cpp19 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(co-cpp19-enable-tests "Build the tests" OFF)
add_subdirectory(third_party)
add_subdirectory(cmake)
if(${co-cpp19-enable-tests})
    enable_testing()    
endif()

add_subdirectory(src/)
