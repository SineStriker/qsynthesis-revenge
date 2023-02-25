# Usage:
# cmake
# -D targets=<target files>
# -D deployqt=<deployqt_exe>
# -D orgdir=<dir>
# -D newdir=<dir>
# -D dummy=<dummy file>
# -P DeployHelper.cmake

# Copy file2 to a new position whose relative path to new_file1 is same as before

include(${CMAKE_CURRENT_LIST_DIR}/Modules/Basic.cmake)

check_defined(targets)
check_defined(deployqt)
check_defined(orgdir)
check_defined(newdir)
check_defined(dummy)

# Message
get_filename_component(SCRIPT_NAME ${CMAKE_CURRENT_LIST_FILE} NAME)
message(STATUS "Run ${SCRIPT_NAME} Script")

# Get new targets'name
set(_new_targets)

foreach(_target ${targets})
    file(RELATIVE_PATH _rel ${orgdir} ${_target})
    list(APPEND _new_targets ${newdir}/${_rel})
endforeach()

file(RELATIVE_PATH _rel ${orgdir} ${dummy})
set(_dummy ${newdir}/${_rel})

# Copy directory
file(REMOVE_RECURSE ${newdir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${orgdir} ${newdir})

# Remove extra files
# foreach(_target ${_new_targets})
# get_filename_component(_name ${_target} NAME_WE)
# get_filename_component(_dir ${_target} DIRECTORY)

# # qzlib.dll
# # qzlib.exp
# # qzlib.lib
# if(WIN32)
# file(GLOB _files ${_dir}/${_name}.*)

# foreach(_file ${_files})
# get_filename_component(_suffix2 ${_file} EXT)
# string(TOLOWER ${_suffix2} _suffix2_lower)

# if(${_suffix2_lower} MATCHES ".*\\.(dll)")
# endif()

# if(NOT ${_suffix2_lower} MATCHES ".*\\.(exe|dll|lib|json)")
# message(${_file})
# file(REMOVE ${_file})
# endif()
# endforeach()
# endif()
# endforeach()
file(GLOB_RECURSE _new_files ${newdir}/**)

foreach(_file ${_new_files})
    get_filename_component(_suffix2 ${_file} EXT)
    string(TOLOWER ${_suffix2} _suffix2_lower)

    if(${_suffix2_lower} MATCHES ".*\\.(lib|exp|pdb|manifest|ilk|a)")
        file(REMOVE ${_file})
    endif()
endforeach()

get_filename_component(_dir ${_dummy} DIRECTORY)
execute_process(
    WORKING_DIRECTORY ${_dir}
    COMMAND ${deployqt}
    --libdir .
    --plugindir plugins
    --no-translations
    --no-system-d3d-compiler

    # --no-virtualkeyboard
    --no-compiler-runtime
    --no-opengl-sw
    --force
    --verbose 0
    ${_dummy}
)