cmake_minimum_required(VERSION 3.17)

project(prebuild_task)

set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/libs" CACHE STRING "The path to use for make install" FORCE)

file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX})

add_subdirectory(src)