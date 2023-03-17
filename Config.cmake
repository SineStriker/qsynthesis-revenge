include_guard(GLOBAL)

# ----------------------------------
# Project directory configuration
# ----------------------------------
set(REPOSITORY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "Repository root directory") # Root dir
set(REPOSITORY_VCPKG_DIR ${REPOSITORY_ROOT_DIR}/vcpkg CACHE STRING "Vcpkg directory") # Vcpkg dir

set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc CACHE STRING "Archives output directory") # Etc
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR} CACHE STRING "Release output directory") # Release

set(PROJECT_MODULES_DIR ${REPOSITORY_ROOT_DIR}/scripts/cmake/modules CACHE STRING "Custom CMake scripts") # Scripts
set(PROJECT_DOCUMENT_DIR ${REPOSITORY_ROOT_DIR}/docs/redist CACHE STRING "Repository documents directory") # Docs
set(PROJECT_LICENSE_FILE ${REPOSITORY_ROOT_DIR}/LICENSE CACHE STRING "License file") # License

# ----------------------------------
# Initialization
# ----------------------------------
include(${PROJECT_MODULES_DIR}/ChorusKitBuildSystem.cmake)

# ----------------------------------
# Build system configuration
# ----------------------------------
ck_option(APP_TOOLSET_NAME ChorusKit)
ck_option(APP_VERSION_VERBOSE 0.0.1.8)
ck_option(APP_START_YEAR 2019)

# Build
ck_option(CONFIG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out-${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
ck_option(CONFIG_INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX})
ck_option(CONFIG_BUILD_TEST on)
ck_option(CONFIG_CREATE_WIN_SHORTCUT on)
ck_option(CONFIG_ENABLE_BREAKPAD off)

# CMake
ck_option(CONFIG_CMAKE_RANDOM_CONFIGURE_FILE off)

# Windows
ck_option(CONFIG_WIN32_DEBUG on)
ck_option(CONFIG_WIN32_MSGBOX_TOPMOST off)