project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(serialize19 REQUIRED)

if(serialize19_FOUND)
   enable_testing()
   include_directories(${serialize19})
   include_directories(${GTest_INCLUDE_DIR})
   file(
      GLOB_RECURSE
      serialize19-tests
      "../../../src/serialize19.lib/*.test.cpp")

   add_executable(cocpp19 ${serialize19-tests})
   target_link_libraries(cocpp19 GTest::gtest_main serialize19::serialize19)
   add_test(cocpp19 cocpp19)
   
endif()



