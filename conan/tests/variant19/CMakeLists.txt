project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(variant19 REQUIRED)
find_package(GTest REQUIRED)
if(variant19_FOUND)
   enable_testing()
   include_directories(${variant19_INCLUDE_DIRS})
   include_directories(${GTest_INCLUDE_DIR})
   file(
      GLOB_RECURSE
      variant19-tests
      "../../../src/variant19.lib/*.test.cpp")

   add_executable(cocpp19 ${variant19-tests})
   target_link_libraries(cocpp19 GTest::gtest_main GTest::gtest variant19::variant19)
   add_test(cocpp19 bin/cocpp19)
   
endif()


