cmake_minimum_required(VERSION 3.15)
project(cocpp19-enum19)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
find_package(GTest REQUIRED)
find_package(CoCpp19 COMPONENTS enum19 REQUIRED)

enable_testing()

include("../../../src/enum19.lib/enum19/enum19.tests.cmake")
