project(conan-cocpp19)
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_STANDARD 20)

find_package(co-cpp19 REQUIRED)
find_package(GTest REQUIRED)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  
  FOREACH(child ${children})
    IF(NOT "${child}" STREQUAL "serialize19.lib")
    message(STATUS "child ${child}")
      IF(IS_DIRECTORY ${curdir}/${child})
         LIST(APPEND dirlist ${child})
      ENDIF()
   ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()



if(co-cpp19_FOUND)
   enable_testing()
   SUBDIRLIST(SUBDIRS "${CMAKE_SOURCE_DIR}/../../../src")
   message(STATUS "Hello world ${SUBDIRS}")
   FOREACH(subdir ${SUBDIRS})
      file(
         GLOB_RECURSE
         testfiles
         "${CMAKE_SOURCE_DIR}/../../../src/${subdir}/*.test.cpp")
      add_executable(${subdir} ${testfiles})

      include_directories(${GTest_INCLUDE_DIR})
      include_directories(${co-cpp19_INCLUDE_DIRS})
      target_link_libraries(${subdir} GTest::gtest_main co-cpp19::co-cpp19)
      add_test(${subdir} ${CMAKE_CURRENT_BINARY_DIR}/bin/${subdir} )
   ENDFOREACH()
   
   
   

   

endif()
