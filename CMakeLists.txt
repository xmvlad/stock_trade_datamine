cmake_minimum_required(VERSION 2.8)

add_definitions(-DQUANT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
add_subdirectory(src)
