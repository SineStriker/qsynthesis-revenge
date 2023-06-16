cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(ChorusKit LANGUAGES CXX)

# ----------------------------------
# Import base configuration
# ----------------------------------
set(ROOT_CMAKE_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake)
set(ROOT_VCPKG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg)

if(NOT NOT_QT_CREATOR)
    set(CK_CONSOLE_ON_DEBUG off)
    set(CK_CONSOLE_ON_RELEASE off)
    set(CK_PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..) # Must be set
endif()

set(CHORUSKIT_REPOSITORY on) # Must be set
set(CK_CMAKE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # Must be set
include(${ROOT_CMAKE_MODULE_DIR}/ChorusKitAPI.cmake)

# ----------------------------------
# Build Options
# ----------------------------------
ck_option(CK_BUILD_TEST on)
ck_option(CK_ENABLE_BREAKPAD off)

ck_option(CK_BUILD_ALL_APPLICATIONS off)
ck_option(CK_BUILD_APPLICATIONS DiffScope) # Not used if `CK_BUILD_ALL_APPLICATIONS` is on

ck_option(CK_BUILD_ALL_TOOLS on)
ck_option(CK_BUILD_TOOLS DiffScope) # Not used if `CK_BUILD_ALL_TOOLS` is on

# ----------------------------------
# Main project
# ----------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------
# Import vcpkg and libraries
# ----------------------------------
ck_init_buildsystem()

# Try get vcpkg
set(_vcpkg_file ${ROOT_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

if(NOT EXISTS ${_vcpkg_file})
    message(WARNING "VCPKG not found, try to get it")
    include(${CK_PROJECT_ROOT_DIR}/scripts/vcpkg/SetupVcpkg.cmake)
endif()

# Tell vcpkg not to copy dlls automatically on Windows
set(VCPKG_APPLOCAL_DEPS off)
include(${_vcpkg_file})
unset(_vcpkg_file)

# Initialize vcpkg
ck_init_vcpkg(${ROOT_VCPKG_DIR} ${VCPKG_TARGET_TRIPLET})

# ----------------------------------
# Build configuration
# ----------------------------------
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

# Find pre-built libraries
find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(ffmpeg-fake REQUIRED NAMES FFmpeg)
find_package(QuaZip REQUIRED NAMES quazip QuaZip-Qt5 QuaZip-Qt6)
find_package(QNrbf REQUIRED)
find_package(QasTool REQUIRED)

if(NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# Add config macros
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    ck_add_definition(CONFIG_BUILD_DEBUG)
endif()

if(NOT WIN32)
    ck_add_definition(CONFIG_DISABLE_FRAMELESS_HELPER)
endif()

ck_add_definition(CONFIG_WIN32_MSGBOX_TOPMOST on)
ck_add_definition(CONFIG_ENABLE_BREAKPAD off)

# ----------------------------------
# Add projects
# ----------------------------------
add_subdirectory(apps)
add_subdirectory(libs)
add_subdirectory(tools)
add_subdirectory(plugins)
add_subdirectory(share)

# Add tests
if(CK_BUILD_TEST)
    add_subdirectory(tests)
endif()

ck_finish_build_system()