cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
project(Tmk)
add_library(Tmk "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk.c"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk.h")
target_include_directories(Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
