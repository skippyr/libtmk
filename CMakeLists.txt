cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
project(tmk)
add_library(tmk "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk.h")
target_include_directories(tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
