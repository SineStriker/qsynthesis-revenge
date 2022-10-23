cmake_minimum_required(VERSION 3.17)

project(QSynthesis CXX)

# User Options
set(USE_PREBUILT_LIBS on)
set(USE_DELAY_LOAD off)
set(BUILD_TEST on)
set(BUILD_PLUGINS_ONLY off)

# CPP Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Application Information
set(APP_DEPLOY OFF)
set(APP_TEST OFF)

# Set Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc)                      # Etc
set(PROJECT_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake) # Scripts
set(PROJECT_RELEASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../release)       # Release

# Determining Current System
set(SYS_WIN OFF)
set(SYS_MAC OFF)
set(SYS_LINUX OFF)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(SYS_WIN ON)
    message(STATUS "[INFO] Current System is Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SYS_LINUX ON)
    message(STATUS "[INFO] Current System is Linux")
elseif(APPLE)
    set(SYS_MAC ON)
    message(STATUS "[INFO] Current System is Macintosh")
else()
    message(FATAL_ERROR "Unsupported System !!!")
endif()

set (PREBUILT_LIBRARY_PATH_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../pkg/libs")

if (USE_PREBUILT_LIBS)
    file(GLOB SDL2_DIR "${PREBUILT_LIBRARY_PATH_PREFIX}/cmake/SDL*")
    file(GLOB QuaZip_DIR "${PREBUILT_LIBRARY_PATH_PREFIX}/cmake/QuaZip*")
    file(GLOB FFmpeg_DIR "${PREBUILT_LIBRARY_PATH_PREFIX}/cmake/FFmpeg*")
    file(GLOB FramelessHelper_DIR "${PREBUILT_LIBRARY_PATH_PREFIX}/cmake/FramelessHelper*")
    file(GLOB yaml-cpp_DIR "${PREBUILT_LIBRARY_PATH_PREFIX}/cmake/yaml-cpp*")
endif()

if (NOT BUILD_PLUGINS_ONLY)
    find_package(SDL2 REQUIRED)
    find_package(QuaZip NAMES QuaZip-Qt5 QuaZip-Qt6 REQUIRED)
    find_package(FFmpeg REQUIRED)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
    find_package(yaml-cpp REQUIRED)
endif()

# Application
add_subdirectory(QsLib)

add_subdirectory(QsPlugin)

if (NOT BUILD_PLUGINS_ONLY)

    add_subdirectory(fsnotifier)

    add_subdirectory(QSynthesis)

endif()

if (BUILD_TEST)

    add_subdirectory(Test)

endif()


