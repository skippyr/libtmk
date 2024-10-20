cmake_minimum_required(VERSION 3.30)
project(Tmk VERSION 17.0.0)
add_library(Tmk "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tmk/Terminal.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Tmk/Terminal.h")
target_include_directories(Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
