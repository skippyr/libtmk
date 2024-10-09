cmake_minimum_required(VERSION 3.20)
project(tmk-tools)
add_subdirectory("${CMAKE_SOURCE_DIR}/.." "${CMAKE_BINARY_DIR}/libtmk")
add_executable(tmk-key-event-logger
               "${CMAKE_SOURCE_DIR}/tmk-key-event-logger.c")
target_link_libraries(tmk-key-event-logger tmk)
install(TARGETS tmk-key-event-logger
        DESTINATION "${CMAKE_SOURCE_DIR}/../build/bin")
