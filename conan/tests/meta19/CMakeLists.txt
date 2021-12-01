project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(meta19 REQUIRED)
find_package(GTest REQUIRED)
if(meta19_FOUND)
   enable_testing()
   include_directories(${meta19_INCLUDE_DIRS})
   include_directories(${GTest_INCLUDE_DIR})
   file(
      GLOB_RECURSE
      meta19-tests
      "../../../src/meta19.lib/*.test.cpp")
      
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fsized-deallocation -Wall -Wextra -Werror --pedantic -ftemplate-backtrace-limit=0 -Wno-gnu-zero-variadic-macro-arguments")
   endif()
   add_executable(cocpp19 ${meta19-tests})
   target_link_libraries(cocpp19 GTest::gtest_main GTest::gtest meta19::meta19)
   add_test(cocpp19 bin/cocpp19)
   
endif()


