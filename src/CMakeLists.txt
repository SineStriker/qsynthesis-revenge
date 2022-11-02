cmake_minimum_required(VERSION 3.17)

project(LabelVoice
    LANGUAGES CXX
    VERSION 0.0.1.3
)

# App Data
set(APP_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(APP_VERSION_SHORT ${APP_VERSION_SHORT})
set(APP_VERSION_VERBOSE ${PROJECT_VERSION})
set(APP_LIB_DIR lib)

# Main Target
set(MAIN_TARGET LabelVoice)

# User Options
set(BUILD_TEST on)
set(BUILD_PLUGINS_ONLY off)

# CPP Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Application Information
if (NOT DEFINED APP_DEPLOY)
    set(APP_DEPLOY OFF)
endif()

# Set Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc)                        # Etc
set(PROJECT_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake)   # Scripts
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR})                            # Release

set(APP_DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../docs/doc) # Doc

# Determining Current System
set(LINUX OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "[INFO] Current System is Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX ON)
    message(STATUS "[INFO] Current System is Linux")
elseif(APPLE)
    message(STATUS "[INFO] Current System is Macintosh")
else()
    message(FATAL_ERROR "Unsupported System !!!")
endif()

# ToolChain
include(${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake)

# SDL2
find_package(SDL2 REQUIRED)

# yaml-cpp
find_package(yaml-cpp REQUIRED)

# FFmpeg
find_package(ffmpeg-fake REQUIRED NAMES FFmpeg)

# FramelessHelper
if (NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# QuaZip
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)

# Application
add_subdirectory(QsLib)

add_subdirectory(QsPlugin)

if (NOT BUILD_PLUGINS_ONLY)

    add_subdirectory(fsnotifier)

    add_subdirectory(${MAIN_TARGET})

endif()

if (BUILD_TEST)
    add_subdirectory(Test)
endif()

if (APP_DEPLOY)
    string(TOLOWER ${MAIN_TARGET} PROJECT_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)

    set(RELEASE_DIRNAME ${PROJECT_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})
    set(APP_DEPLOY_DIR ${PROJECT_RELEASE_DIR}/${RELEASE_DIRNAME})

    set(FSNOTIFIER_TARGET fsnotifier)

    add_custom_target(deploy
        DEPENDS all
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${APP_DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${APP_DOC_DIR} ${APP_DEPLOY_DIR}/doc
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${MAIN_TARGET}>/lib ${APP_DEPLOY_DIR}/lib
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${MAIN_TARGET}>/plugins ${APP_DEPLOY_DIR}/plugins
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MAIN_TARGET}> ${APP_DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FSNOTIFIER_TARGET}> ${APP_DEPLOY_DIR}
    )
endif()
