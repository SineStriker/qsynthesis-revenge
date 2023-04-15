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
ck_option(CONFIG_APP_VERSION 0.0.1.8)
ck_option(CONFIG_START_YEAR 2019)
ck_option(CONFIG_CURRENT_APP DiffScope)

# Build
ck_option(CONFIG_BUILD_TEST on)
ck_option(CONFIG_ENABLE_DEVELOP off)
ck_option(CONFIG_ENABLE_BREAKPAD off)

# CMake
# ck_option(CONFIG_CMAKE_RANDOM_CONFIGURE_FILE off)

# Windows
ck_option(CONFIG_WIN32_DEBUG on) # Enable debug by default
ck_option(CONFIG_WIN32_NO_DEBUG on) # Disable debug when build type is release
ck_option(CONFIG_WIN32_MSGBOX_TOPMOST off) # Set message box topest flag