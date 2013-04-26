cmake_minimum_required(VERSION 2.8)
project(CompGeo)

find_package(Boost COMPONENTS system REQUIRED)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions("-DEIGEN_DEFAULT_TO_ROW_MAJOR")
set(BUILD_SHARED_LIBS off)

add_subdirectory(lib)
add_subdirectory(src)

