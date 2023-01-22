cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Import configuration
include(../Config.cmake)

# Project
project(${APP_TOOLSET_NAME} VERSION ${APP_VERSION_VERBOSE} LANGUAGES CXX)

# Language configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set global variables
set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING INTERNAL)

# Include global module
add_subdirectory(Common)

# Set build variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
endif()

# Import vcpkg
set(VCPKG_APPLOCAL_DEPS off)
include(${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake)

# Import utils
include(${PROJECT_MODULES_DIR}/Basic.cmake)
include(${PROJECT_MODULES_DIR}/Deploy.cmake)
include(${PROJECT_MODULES_DIR}/FindUtil.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)
include(${PROJECT_MODULES_DIR}/Target.cmake)

# Find pre-built libraries
find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(ffmpeg-fake REQUIRED NAMES FFmpeg)
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)

if(NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# Add qslib
add_subdirectory(QsLib)

# Add applications
add_subdirectory(Utilities)

foreach(_app ${APP_TOOLSET_LIST})
    add_subdirectory(${_app})
endforeach()

# Add tests
if(BUILD_TEST)
    add_subdirectory(Test)
endif()

# Subsequent configurations
if(TRUE)
    get_all_targets(_all_targets)

    proc_deps(${_all_targets})

    if(APP_DEPLOY)
        proc_deploy(${_all_targets})
    endif()
endif()
