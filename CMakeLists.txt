cmake_minimum_required(VERSION 3.20)
project(tmk)
add_library(tmk "${CMAKE_CURRENT_SOURCE_DIR}/src/tmk.c" "${CMAKE_CURRENT_SOURCE_DIR}/src/tmk.h")
target_include_directories(tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
