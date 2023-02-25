include_guard(GLOBAL)

# ----------------------------------
# Project directory configuration
# ----------------------------------
set(REPOSITORY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "Repository root directory") # Root dir
set(REPOSITORY_VCPKG_DIR ${REPOSITORY_ROOT_DIR}/vcpkg CACHE STRING "Vcpkg directory") # Vcpkg dir

set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc CACHE STRING "Archives output directory") # Etc
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR} CACHE STRING "Release output directory") # Release

set(PROJECT_MODULES_DIR ${REPOSITORY_ROOT_DIR}/scripts/cmake/Modules CACHE STRING "Custom CMake scripts") # Scripts
set(PROJECT_DOCUMENT_DIR ${REPOSITORY_ROOT_DIR}/docs/redist CACHE STRING "Repository documents directory") # Docs
set(PROJECT_LICENSE_FILE ${REPOSITORY_ROOT_DIR}/LICENSE CACHE STRING "License file") # License

# ----------------------------------
# Initialization
# ----------------------------------
include(${PROJECT_MODULES_DIR}/Basic.cmake)

check_pc_os()

# ----------------------------------
# Build system configuration
# ----------------------------------
option_ex(APP_TOOLSET_NAME ChorusKit)
option_ex(APP_VERSION_VERBOSE 0.0.1.8)

# Build
option_ex(CONFIG_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out-${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
option_ex(CONFIG_INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX})
option_ex(CONFIG_BUILD_TEST on)

# Windows
option_ex(CONFIG_WIN32_MSGBOX_TOPMOST off)
option_ex(CONFIG_WIN32_DEBUG on)

# CMake
option_ex(CONFIG_CMAKE_RANDOM_CONFIGURE_FILE off)