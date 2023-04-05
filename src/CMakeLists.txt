cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ----------------------------------
# Import base configuration
# ----------------------------------
include(../Config.cmake)

# ----------------------------------
# Main project
# ----------------------------------
project(ChorusKit VERSION ${CONFIG_APP_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------
# Root settings
# ----------------------------------
set(ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ROOT_OPENVPI_COPYRIGHT "Copyright ${CONFIG_START_YEAR}-${ROOT_CURRENT_YEAR} OpenVPI Team")

string(TOLOWER ${CONFIG_CURRENT_APP} ROOT_CURRENT_APP_LOWER)
string(TIMESTAMP ROOT_CURRENT_YEAR %Y)

# ----------------------------------
# Import vcpkg and libraries
# ----------------------------------
ck_init_buildsystem(
    NAME "ChorusKit ${CONFIG_CURRENT_APP}"
    VERSION ${PROJECT_VERSION}
    DOCS ../LICENSE ../docs/redist/thirdparty
)

set(VCPKG_APPLOCAL_DEPS off) # Tell vcpkg not to copy dlls automatically on Windows
include(${REPOSITORY_VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

ck_init_vcpkg(${REPOSITORY_VCPKG_DIR} ${VCPKG_TARGET_TRIPLET})

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
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)
find_package(QNrbf REQUIRED)
find_package(QasTool REQUIRED)

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

if(CONFIG_ENABLE_BREAKPAD)
    add_compile_definitions(CONFIG_ENABLE_BREAKPAD)
endif()

if((CONFIG_WIN32_DEBUG OR NOT_QT_CREATOR) AND(NOT CONFIG_WIN32_NO_DEBUG OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(ROOT_APPMAIN_TYPE CONSOLE)
else()
    set(ROOT_APPMAIN_TYPE WINDOWS)
endif()

# ----------------------------------
# Add projects
# ----------------------------------
add_subdirectory(apps)
add_subdirectory(libs)
add_subdirectory(tools)
add_subdirectory(plugins)

# Add tests
if(CONFIG_BUILD_TEST)
    add_subdirectory(tests)
endif()

ck_finish_build_system()