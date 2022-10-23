# Open the project in src is suggested.

cmake_minimum_required(VERSION 3.17)

project(QSynthesis LANGUAGES CXX)

set(PROJECT_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake) # Scripts
set(PROJECT_ARCHIVE_DIR ${PROJECT_BINARY_DIR}/etc) # Etc
set(PROJECT_DEPLOY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/release) # Release

set(NOT_QT_CREATOR 1)

add_subdirectory(src)