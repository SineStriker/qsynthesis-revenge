# Usage:
# cmake
# -D src=<files/dirs>
# -P dest=<dir>

# copy file or dir to dest if different

# From CMake 3.26, `copy_directory_if_different` feature is supported,
# this script is intended to be compatible with earlier versions.

include(${CMAKE_CURRENT_LIST_DIR}/../ChorusKitUtils.cmake)

ck_check_defined(src)
ck_check_defined(dest)

# Message
# get_filename_component(SCRIPT_NAME ${CMAKE_CURRENT_LIST_FILE} NAME)
# message(STATUS "Run ${SCRIPT_NAME} Script")
if(NOT IS_ABSOLUTE ${dest})
    get_filename_component(_dest ${dest} ABSOLUTE)
else()
    set(_dest ${dest})
endif()

set(_cnt 0)

function(_copy_if_different _file _target_file)
    get_filename_component(_target_dir ${_target_file} DIRECTORY)

    # Remove if not file
    if(IS_DIRECTORY ${_target_file})
        file(REMOVE_RECURSE ${_target_file})
    endif()

    # Compare file time
    if(NOT EXISTS ${_target_file} OR ${_file} IS_NEWER_THAN ${_target_file})
        # Remove if not dir
        if(EXISTS ${_target_dir} AND NOT IS_DIRECTORY ${_target_dir})
            file(REMOVE ${_target_dir})
        endif()

        # Make dir if not exist
        if(NOT EXISTS ${_target_dir})
            make_directory(${_target_dir})
        endif()

        # Copy file
        # file(COPY ${_file} DESTINATION ${_target_dir})

        # Use cmake command becault file(COPY) doesn't update modification time
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${_target_dir})

        message(STATUS "Copy \"${_file}\" to \"${_target_dir}\"")
        math(EXPR _cnt "${_cnt} + 1")
        set(_cnt ${_cnt} PARENT_SCOPE)
    endif()
endfunction()

foreach(_item ${src})
    if(NOT IS_ABSOLUTE ${_item})
        get_filename_component(_item ${_item} ABSOLUTE)
    endif()

    if(IS_DIRECTORY ${_item})
        get_filename_component(_name ${_item} NAME)
        file(GLOB_RECURSE _files ${_item}/*)

        foreach(_file ${_files})
            file(RELATIVE_PATH _rel_path ${_item} ${_file})
            _copy_if_different(${_file} ${_dest}/${_name}/${_rel_path})
        endforeach()
    else()
        file(GLOB _files ${_item})

        foreach(_file ${_files})
            get_filename_component(_name ${_file} NAME)
            _copy_if_different(${_file} ${_dest}/${_name})
        endforeach()
    endif()
endforeach()

if(${_cnt} GREATER 0)
    message(STATUS "Copy ${_cnt} files in total.")
endif()