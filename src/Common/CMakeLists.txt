
# ----------------- Pre-process -----------------
set(_templates_dir ${CMAKE_CURRENT_LIST_DIR}/templates)
string(TIMESTAMP _current_year %Y)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(_suffix "/debug")
else()
    set(_suffix)
endif()

if(WIN32)
    set(_bin_dir "${ROOT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/bin")
    set(_bin_pat "*.dll")
elseif(APPLE)
    set(_bin_dir "${ROOT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/lib")
    set(_bin_pat "*.dylib")
else()
    set(_bin_dir "${ROOT_SOURCE_DIR}/../vcpkg/installed/${VCPKG_TARGET_TRIPLET}${_suffix}/lib")
    set(_bin_pat "*.so*")
endif()

# ----------------- Global variables -----------------
# Time
set(TIME_PROJECT_START_YEAR     2020                                                CACHE STRING INTERNAL)
set(TIME_CURRENT_YEAR           ${_current_year}                                    CACHE STRING INTERNAL)

# Resource files
set(WIN32_DLL_RC                ${_templates_dir}/Win32DllRes.rc.in                 CACHE STRING INTERNAL)
set(WIN32_EXE_RC                ${_templates_dir}/Win32ExeRes.rc.in                 CACHE STRING INTERNAL)
set(WIN32_EXE_MANIFEST          ${_templates_dir}/Win32ExeManifest.manifest.in      CACHE STRING INTERNAL)
set(QT_CONF_SETTING             ${_templates_dir}/qt.conf.in                        CACHE STRING INTERNAL)

# Set project files or dirs
set(PROJECT_ARCHIVE_DIR         ${CMAKE_BINARY_DIR}/etc                             CACHE STRING INTERNAL) # Etc
set(PROJECT_MODULES_DIR         ${ROOT_SOURCE_DIR}/../scripts/cmake/Modules         CACHE STRING INTERNAL) # Scripts
set(PROJECT_RELEASE_DIR         ${CMAKE_BINARY_DIR}                                 CACHE STRING INTERNAL) # Release
set(PROJECT_DOCUMENT_DIR        ${ROOT_SOURCE_DIR}/../docs/redist                   CACHE STRING INTERNAL) # Docs
set(PROJECT_LICENSE_FILE        ${ROOT_SOURCE_DIR}/../LICENSE                       CACHE STRING INTERNAL) # License

# Vcpkg
set(VCPKG_BINARY_DIR            ${_bin_dir}                                         CACHE STRING INTERNAL)
set(VCPKG_BINARY_PAT            ${_bin_pat}                                         CACHE STRING INTERNAL)

# ----------------- Initialization -----------------
# Include all scripts
file(GLOB_RECURSE _scripts ${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake)

foreach(_script ${_scripts})
    include(${_script})
endforeach()

# Add sub dirs
add_subdirectory(src)
