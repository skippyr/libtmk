cmake_minimum_required(VERSION 3.20)
project(Tmk)
set(CMAKE_CXX_STANDARD 23)
add_library(Tmk "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/AnsiColor.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/CommandLineArguments.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/CommandLineArguments.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Coordinate.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Coordinate.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/CursorShape.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Dimensions.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Dimensions.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/EncodingConverter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/EncodingConverter.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Exception.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/FontEffect.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/FontWeight.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Layer.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/OutOfRangeException.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/RgbColor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/RgbColor.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/StreamRedirectionException.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/System.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Terminal.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmk/Terminal.h")
target_include_directories(Tmk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
