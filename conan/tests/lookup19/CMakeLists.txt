project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(lookup19 REQUIRED)

add_executable(cocpp19 main.cpp)

if(lookup19_FOUND)
   include_directories(${lookup19_INCLUDE_DIRS})
   target_link_libraries(cocpp19 lookup19::lookup19)
endif()



