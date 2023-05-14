include_guard(GLOBAL)

include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitUtils.cmake)

set(CK_CMAKE_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CK_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/commands)

# ----------------------------------
# Build system configuration
# ----------------------------------
set(CK_MAIN_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out-${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})

# Meta
set(CK_ARCHIVE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/etc)
set(CK_HEADERS_OUTPUT_PATH ${CK_ARCHIVE_OUTPUT_PATH}/include)

# Build
if(APPLE)
    set(_CK_GLOBAL_BASE_OUTPUT_PATH ${CK_MAIN_OUTPUT_PATH}/shared)
    set(CK_GLOBAL_RUNTIME_OUTPUT_PATH ${_CK_GLOBAL_BASE_OUTPUT_PATH}/MacOS)
    set(CK_GLOBAL_LIBRARY_OUTPUT_PATH ${_CK_GLOBAL_BASE_OUTPUT_PATH}/Frameworks)
    set(CK_GLOBAL_SHARE_OUTPUT_PATH ${_CK_GLOBAL_BASE_OUTPUT_PATH}/Resources)
    set(CK_QT_CONF_OUTPUT_DIR ${_CK_GLOBAL_BASE_OUTPUT_PATH}/Resources)

    set(_CK_INSTALL_BASE_OUTPUT_PATH shared)
    set(CK_INSTALL_RUNTIME_OUTPUT_PATH ${_CK_INSTALL_BASE_OUTPUT_PATH}/MacOS)
    set(CK_INSTALL_LIBRARY_OUTPUT_PATH ${_CK_INSTALL_BASE_OUTPUT_PATH}/Frameworks)
    set(CK_INSTALL_SHARE_OUTPUT_PATH ${_CK_INSTALL_BASE_OUTPUT_PATH}/Resources)

    set(CK_PLATFORM_NAME Macintosh)
    set(CK_PLATFORM_LOWER mac)
else()
    set(CK_GLOBAL_RUNTIME_OUTPUT_PATH ${CK_MAIN_OUTPUT_PATH}/bin)
    set(CK_GLOBAL_LIBRARY_OUTPUT_PATH ${CK_MAIN_OUTPUT_PATH}/lib)
    set(CK_GLOBAL_SHARE_OUTPUT_PATH ${CK_MAIN_OUTPUT_PATH}/share)
    set(CK_QT_CONF_OUTPUT_DIR ${CK_MAIN_OUTPUT_PATH}/bin)

    set(CK_INSTALL_RUNTIME_OUTPUT_PATH bin)
    set(CK_INSTALL_LIBRARY_OUTPUT_PATH lib)
    set(CK_INSTALL_SHARE_OUTPUT_PATH share)

    if(WIN32)
        set(CK_PLATFORM_NAME Windows)
        set(CK_PLATFORM_LOWER win)
    elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
        set(LINUX true CACHE BOOL "Linux System" FORCE)
        set(CK_PLATFORM_NAME Linux)
        set(CK_PLATFORM_LOWER linux)
    else()
        message(FATAL_ERROR "Unsupported System !!!")
    endif()
endif()

# Install
set(CK_INSTALL_INCLUDE_DIR include/ChorusKit)
set(CK_INSTALL_CMAKE_DIR lib/cmake/ChorusKit)
set(CK_INSTALL_EXPORT ChorusKitTargets)

# ----------------------------------
# Public configuration
# ----------------------------------
# Metadata
ck_option(CK_CURRENT_VERSION 0.0.1.8)

# CMake
ck_option(CK_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
ck_option(CK_PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

# Build
ck_option(CK_CONSOLE_ON_DEBUG on)
ck_option(CK_CONSOLE_ON_RELEASE off)

# ----------------------------------
# Repository configuration
# ----------------------------------
if(NOT CHORUSKIT_REPOSITORY)
    return()
endif()

set(CK_PYTHON_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/../python)

# Mode
ck_option(CK_ENABLE_DEVEL off)
ck_option(CK_ENABLE_DEPLOY_QT on)
ck_option(CK_ENABLE_VCPKG_DEPS on)

# CMake
ck_option(CK_CMAKE_RANDOM_LENGTH 8)

# Build
ck_option(CK_BUILD_TEST on)
ck_option(CK_ENABLE_BREAKPAD off)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Host system: ${CK_PLATFORM_NAME}, ${CMAKE_HOST_SYSTEM}")