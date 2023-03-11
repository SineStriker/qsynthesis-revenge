cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

include(../Config.cmake)

# ----------------------------------
# Main project
# ----------------------------------
project(${APP_TOOLSET_NAME} VERSION ${APP_VERSION_VERBOSE} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------
# Import vcpkg and libraries
# ----------------------------------
ck_init_buildsystem() # Init choruskit buildsystem

set(VCPKG_APPLOCAL_DEPS off) # Tell vcpkg not to copy dlls automatically on Windows
include(${REPOSITORY_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

ck_init_vcpkg(${REPOSITORY_VCPKG_DIR} ${VCPKG_TARGET_TRIPLET})

# ----------------------------------
# Import base configuration
# ----------------------------------
set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------
# Build configuration
# ----------------------------------
set(ROOT_SHARE_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}/share)

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
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)
find_package(QNrbf REQUIRED)
find_package(qastool REQUIRED)

if(NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# Add config macros
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(CONFIG_ENABLE_DEBUG)
endif()

if(CONFIG_WIN32_MSGBOX_TOPMOST)
    add_compile_definitions(CONFIG_WIN32_MSGBOX_TOPMOST)
endif()

if(CONFIG_WIN32_DEBUG OR NOT_QT_CREATOR)
    set(ROOT_APPMAIN_TYPE CONSOLE)
else()
    set(ROOT_APPMAIN_TYPE WINDOWS)
endif()

string(TIMESTAMP ROOT_CURRENT_YEAR %Y)
set(ROOT_OPENVPI_COPYRIGHT "Copyright ${APP_START_YEAR}-${ROOT_CURRENT_YEAR} OpenVPI Team")

# ----------------------------------
# Add projects
# ----------------------------------
add_subdirectory(libs)
add_subdirectory(tools)
add_subdirectory(plugins)
add_subdirectory(apps)

# Add tests
if(CONFIG_BUILD_TEST)
    add_subdirectory(tests)
endif()