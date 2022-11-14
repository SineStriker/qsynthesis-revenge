cmake_minimum_required(VERSION 3.17)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Import configuration
include(../Config.cmake)

# Project
project(${MAIN_TARGET} VERSION ${APP_VERSION_VERBOSE} LANGUAGES CXX)

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

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Build .so/.dylib files at bin-dir
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set project information
set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc) # Etc
set(PROJECT_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/cmake/Modules) # Scripts
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR}) # Release
set(PROJECT_DOCUMENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../docs/doc) # Doc
set(PROJECT_LICENSE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE) # License
set(PROJECT_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# Set global variables
set(TIME_PROJECT_START_YEAR 2020)
string(TIMESTAMP TIME_CURRENT_YEAR %Y)
set(MACOSX_BUNDLE_PLIST ${PROJECT_MODULES_DIR}/MacOSXBundleInfo.plist.in)
set(MACOSX_FRAMEWORK_PLIST ${PROJECT_MODULES_DIR}/MacOSXFrameworkInfo.plist.in)
set(WIN32_DLL_RC ${PROJECT_MODULES_DIR}/Win32DllRes.rc.in)
set(WIN32_EXE_RC ${PROJECT_MODULES_DIR}/Win32ExeRes.rc.in)

# ToolChain
set(VCPKG_APPLOCAL_DEPS off)
include(${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake)

# Import utils
include(${PROJECT_MODULES_DIR}/Basic.cmake)
include(${PROJECT_MODULES_DIR}/FindUtil.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

# Find pre-built libraries
find_package(SDL2 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(ffmpeg-fake REQUIRED NAMES FFmpeg)
find_package(QuaZip NAMES quazip QuaZip-Qt5 QuaZip-Qt6 REQUIRED)

if(NOT APPLE)
    find_package(FramelessHelper REQUIRED COMPONENTS Core Widgets)
endif()

# Add subdirectories
add_subdirectory(QsLib)

if(NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(fsnotifier)
    add_subdirectory(${MAIN_TARGET})
endif()

if(BUILD_TEST)
    add_subdirectory(Test)
endif()

# Add deploy target
if(APP_DEPLOY)
    message(STATUS "[INFO] Deployment mode, virtual target is added")

    if(NOT WIN32)
        message(FATAL_ERROR "Deployment mode is only implemented on Windows!")
    endif()

    string(TOLOWER ${MAIN_TARGET} PROJECT_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)

    set(RELEASE_DIRNAME ${PROJECT_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})
    set(APP_DEPLOY_DIR ${PROJECT_RELEASE_DIR}/${RELEASE_DIRNAME})

    set(FSNOTIFIER_TARGET fsnotifier)

    add_custom_target(
        deploy
        DEPENDS all
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${APP_DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_DEPLOY_DIR}

        # Copy lib dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${MAIN_TARGET}>/${APP_LIB_DIR}
        ${APP_DEPLOY_DIR}/${APP_LIB_DIR}

        # Copy plugins dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${MAIN_TARGET}>/${APP_PLUGINS_DIR}
        ${APP_DEPLOY_DIR}/${APP_PLUGINS_DIR}

        # Copy res dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${MAIN_TARGET}>/${APP_RES_DIR}
        ${APP_DEPLOY_DIR}/${APP_RES_DIR}

        # Copy docs dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_DOCUMENT_DIR} ${APP_DEPLOY_DIR}/${APP_RES_DIR}/docs

        # Copy license
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_LICENSE_FILE} ${APP_DEPLOY_DIR}/${APP_RES_DIR}/docs
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MAIN_TARGET}> ${APP_DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${FSNOTIFIER_TARGET}> ${APP_DEPLOY_DIR}
    )
endif()
