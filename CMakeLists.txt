cmake_minimum_required(VERSION 3.20)
project(Tmk)
set(CMAKE_CXX_STANDARD 23)
add_library(Tmk
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Terminal.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Terminal.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/System.h")
target_include_directories(Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
