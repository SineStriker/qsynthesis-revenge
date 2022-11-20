cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Import configuration
include(../Config.cmake)

# Project
project(${APP_TOOLSET_NAME} VERSION ${APP_VERSION_VERBOSE} LANGUAGES CXX)

# Language configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check operating system
set(LINUX OFF)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX ON)
    message(STATUS "[INFO] Current System is Linux")
elseif(WIN32)
    message(STATUS "[INFO] Current System is Windows")
elseif(APPLE)
    message(STATUS "[INFO] Current System is Macintosh")
else()
    message(FATAL_ERROR "Unsupported System !!!")
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set project information
set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc) # Etc
set(PROJECT_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake/Modules) # Scripts
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR}) # Release
set(PROJECT_DOCUMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../docs/doc) # Doc
set(PROJECT_LICENSE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE) # License

# Set global variables
set(TIME_PROJECT_START_YEAR 2020)
string(TIMESTAMP TIME_CURRENT_YEAR %Y)
set(MACOSX_BUNDLE_PLIST ${PROJECT_MODULES_DIR}/MacOSXBundleInfo.plist.in)
set(MACOSX_FRAMEWORK_PLIST ${PROJECT_MODULES_DIR}/MacOSXFrameworkInfo.plist.in)
set(WIN32_DLL_RC ${PROJECT_MODULES_DIR}/Win32DllRes.rc.in)
set(WIN32_EXE_RC ${PROJECT_MODULES_DIR}/Win32ExeRes.rc.in)
set(WIN32_EXE_MANIFEST ${PROJECT_MODULES_DIR}/Win32ExeManifest.manifest.in)

# Vcpkg
set(VCPKG_APPLOCAL_DEPS off)
include(${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake)

# Import utils
include(${PROJECT_MODULES_DIR}/Basic.cmake)
include(${PROJECT_MODULES_DIR}/FindUtil.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/Linguist.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

# Find pre-built libraries
find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(ffmpeg-fake REQUIRED NAMES FFmpeg)
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)

if(NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# Add common
add_subdirectory(Common)
include(Common/Include.cmake)

# Add qslib
add_subdirectory(QsLib)

# Add applications
if(NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(Utilities)

    foreach(_app ${APP_TOOLSET_LIST})
        add_subdirectory(${_app})
    endforeach()
endif()

if(BUILD_TEST)
    add_subdirectory(Test)
endif()

# Add other tasks
if(TRUE)
    set(_suffix)

    if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
        set(_suffix "/debug")
    endif()

    if(WIN32)
        set(VCPKG_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/bin")
        set(VCPKG_BINARY_PAT "*.dll")
    elseif(APPLE)
        set(VCPKG_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/lib")
        set(VCPKG_BINARY_PAT "*.dylib")
    else()
        set(VCPKG_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/lib")
        set(VCPKG_BINARY_PAT "*.so*")
    endif()

    unset(_suffix)

    # Get all targets in variable CURRENT_ALL_TARGETS
    get_all_targets(CURRENT_ALL_TARGETS)

    # Copy dlls and and add translations
    include(Deps.cmake)

    if(APP_DEPLOY)
        include(Deploy.cmake)
    endif()

    unset(CURRENT_ALL_TARGETS)
endif()
