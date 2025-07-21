cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
project(Tmtk)
add_library(Tmtk "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmtk.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/Tmtk.hpp")
target_include_directories(Tmtk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
