include_guard(GLOBAL)

# ----------------------------------
# Project directory configuration
# ----------------------------------
set(REPOSITORY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE STRING INTERNAL) # Root dir
set(REPOSITORY_VCPKG_DIR ${REPOSITORY_ROOT_DIR}/vcpkg CACHE STRING INTERNAL) # Vcpkg dir

set(PROJECT_ARCHIVE_DIR ${CMAKE_BINARY_DIR}/etc CACHE STRING INTERNAL) # Etc
set(PROJECT_RELEASE_DIR ${CMAKE_BINARY_DIR} CACHE STRING INTERNAL) # Release

set(PROJECT_MODULES_DIR ${REPOSITORY_ROOT_DIR}/scripts/cmake/Modules CACHE STRING INTERNAL) # Scripts
set(PROJECT_DOCUMENT_DIR ${REPOSITORY_ROOT_DIR}/docs/redist CACHE STRING INTERNAL) # Docs
set(PROJECT_LICENSE_FILE ${REPOSITORY_ROOT_DIR}/LICENSE CACHE STRING INTERNAL) # License


# ----------------------------------
# Check environment
# ----------------------------------
include(${PROJECT_MODULES_DIR}/Basic.cmake)

check_pc_os()


# ----------------------------------
# Build system configuration
# ----------------------------------
option_ex(APP_TOOLSET_NAME ChorusKit)
option_ex(APP_TOOLSET_LIST DiffSinger)
option_ex(APP_VERSION_VERBOSE 0.0.1.7)
option_ex(APP_DEPLOY off)
option_ex(APP_LIB_DIR lib)
option_ex(APP_TOOLS_DIR bin)
option_ex(APP_PLUGINS_DIR plugins) # Do not change
option_ex(APP_LOCALES_DIR translations) # Do not change
option_ex(APP_RES_DIR resources)
option_ex(APP_PUBLIC_PLUGINS_DIR qsplugins)

option_ex(BUILD_TEST off)

option_ex(CONFIG_WIN32_DEBUG on)
