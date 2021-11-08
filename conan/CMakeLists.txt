cmake_minimum_required(VERSION 3.15)
project(hello CXX)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
add_subdirectory(${PARENT_DIR}/ build)
