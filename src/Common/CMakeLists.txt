set(_templates_dir ${CMAKE_CURRENT_LIST_DIR}/templates)
set(_vcpkg_installed_dir ${REPOSITORY_VCPKG_DIR}/installed)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(_vcpkg_triplet_suffix "/debug")
else()
    set(_vcpkg_triplet_suffix)
endif()

string(TIMESTAMP _current_year %Y)

# VCPKG_TARGET_TRIPLET is defined inside vcpkg scripts
if(WIN32)
    set(_bin_dir ${_vcpkg_installed_dir}/${VCPKG_TARGET_TRIPLET}${_vcpkg_triplet_suffix}/bin)
    set(_bin_pat "*.dll")
elseif(APPLE)
    set(_bin_dir ${_vcpkg_installed_dir}/${VCPKG_TARGET_TRIPLET}${_vcpkg_triplet_suffix}/lib)
    set(_bin_pat "*.dylib")
else()
    set(_bin_dir ${_vcpkg_installed_dir}/${VCPKG_TARGET_TRIPLET}${_vcpkg_triplet_suffix}/lib)
    set(_bin_pat "*.so*")
endif()

# ----------------------------------
# Set global variables
# ----------------------------------
# Time
set(TIME_PROJECT_START_YEAR     2020                                                CACHE STRING "" FORCE)
set(TIME_CURRENT_YEAR           ${_current_year}                                    CACHE STRING "" FORCE)

# Resource files
set(WIN32_DLL_RC                ${_templates_dir}/Win32DllRes.rc.in                 CACHE STRING "" FORCE)
set(WIN32_EXE_RC                ${_templates_dir}/Win32ExeRes.rc.in                 CACHE STRING "" FORCE)
set(WIN32_EXE_MANIFEST          ${_templates_dir}/Win32ExeManifest.manifest.in      CACHE STRING "" FORCE)
set(QT_CONF_SETTING             ${_templates_dir}/qt.conf.in                        CACHE STRING "" FORCE)

# Vcpkg
set(VCPKG_BINARY_DIR            "${_bin_dir}"                                       CACHE STRING "" FORCE)
set(VCPKG_BINARY_PAT            "${_bin_pat}"                                       CACHE STRING "" FORCE)

# ----------------------------------
# Initialization
# ----------------------------------
# Include all scripts
file(GLOB_RECURSE _scripts ${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake)

foreach(_script ${_scripts})
    include(${_script})
endforeach()

# Include common utils
include(${PROJECT_MODULES_DIR}/Deploy.cmake)
include(${PROJECT_MODULES_DIR}/FindUtil.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)
include(${PROJECT_MODULES_DIR}/Target.cmake)

# Add sub dirs
add_subdirectory(src)