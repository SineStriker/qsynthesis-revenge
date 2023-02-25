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
set(VCPKG_APPLOCAL_DEPS off) # Tell vcpkg not to copy dlls automatically on Windows
include(${REPOSITORY_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

# ----------------------------------
# Import base configuration
# ----------------------------------
set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Root source directory")
add_subdirectory(shared)

# ----------------------------------
# Build configuration
# ----------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}/lib)

set(SHARE_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}/share)

if(WIN32)
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

    # add_subdirectory(Test)
    # add_subdirectory(TestNew)
endif()

# ----------------------------------
# Add dependencies and deployment
# ----------------------------------
if(TRUE)
    get_all_targets(_all_targets)
    proc_deps("${_all_targets}")
    proc_deploy2("${_all_targets}")
endif()