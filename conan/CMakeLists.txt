cmake_minimum_required(VERSION 3.15)
project(co-cpp19 CXX)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
add_subdirectory(${PARENT_DIR}/ build)
