cmake_minimum_required (VERSION 3.20)
project (Tmk VERSION 17.0.0
             DESCRIPTION "A cross-platform terminal manipulation library for C++20 with features to manipulate terminal properties, styles and key readings using UTF-8 encoding."
             HOMEPAGE_URL "https://github.com/skippyr/Tmk"
             LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
file (GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Source Files/*.cpp")
message ("Adding source files to Tmk:")

foreach (file ${SOURCE_FILES})
    message("    - ${file}")
endforeach ()

add_library (Tmk ${SOURCE_FILES})
message ("Adding header files directory to Tmk: \"${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Header Files\"")
target_include_directories (Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/Header Files")
