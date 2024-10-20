cmake_minimum_required(VERSION 3.30)
project(Tmk VERSION 17.0.0)

set(CMAKE_CXX_STANDARD 20)
set(SOURCES_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
file(GLOB_RECURSE SOURCE_FILES "${SOURCES_DIRECTORY}/*.cpp" "${SOURCES_DIRECTORY}/*.h")

add_library(Tmk ${SOURCE_FILES})
target_include_directories(Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
