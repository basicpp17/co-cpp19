project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(partial19 REQUIRED)

add_executable(cocpp19 main.cpp)

if(partial19_FOUND)
   include_directories(${partial19_INCLUDE_DIRS})
   target_link_libraries(cocpp19 partial19::partial19)
endif()



