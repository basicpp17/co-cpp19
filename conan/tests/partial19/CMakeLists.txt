project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(partial19 REQUIRED)
find_package(GTest REQUIRED)
if(partial19_FOUND)
   enable_testing()
   include_directories(${partial19_INCLUDE_DIRS})
   include_directories(${GTest_INCLUDE_DIR})
   file(
      GLOB_RECURSE
      partial19-tests
      "../../../src/partial19.lib/*.test.cpp")

   add_executable(cocpp19 ${partial19-tests})
   target_link_libraries(cocpp19 GTest::gtest_main GTest::gtest partial19::partial19)
   add_test(cocpp19 bin/cocpp19)
   
endif()


