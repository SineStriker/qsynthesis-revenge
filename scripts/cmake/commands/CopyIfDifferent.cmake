# Usage:
# cmake
# -D src=<files/dirs>
# -D dest=<dir>
# -P CopyIfDifferent.cmake

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

function(_remove_target_symlink _item _target)
    # Remove if target is symlink and source is not symlink
    if(IS_SYMLINK ${_target} AND NOT IS_SYMLINK ${_item})
        file(REMOVE ${_target})
    endif()
endfunction()

function(_copy_if_different _file _target_file)
    get_filename_component(_target_dir ${_target_file} DIRECTORY)

    # Remove if target is symlink and source is not symlink
    _remove_target_symlink(${_file} ${_target_file})

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
            file(MAKE_DIRECTORY ${_target_dir})
        endif()

        # Copy file (symlinks are not followed)
        # Note: file(COPY) doesn't update modification time
        file(COPY ${_file} DESTINATION ${_target_dir})

        # Update modification/access time of target file
        file(TOUCH_NOCREATE ${_target_file})

        message(STATUS "Copy \"${_file}\" to \"${_target_dir}\"")
        math(EXPR _cnt "${_cnt} + 1")
        set(_cnt ${_cnt} PARENT_SCOPE)
    endif()
endfunction()

macro(_copy_dir _item)
    get_filename_component(_name ${_item} NAME)

    # Set the CMake policy CMP0009 to NEW
    # CMP0009: FILE GLOB_RECURSE calls should not follow symlinks by default
    cmake_policy(PUSH)
    cmake_policy(SET CMP0009 NEW)

    # Get a list of all files in the directory to be copied
    # (symlinks will not be followed)
    file(GLOB_RECURSE _files LIST_DIRECTORIES true ${_item}/*)

    # Restore the CMake policy
    cmake_policy(POP)

    foreach(_file ${_files})
        file(RELATIVE_PATH _rel_path ${_item} ${_file})
        if(NOT IS_SYMLINK ${_file} AND IS_DIRECTORY ${_file})
            # If we set `LIST_DIRECTORIES true` in `GLOB_RECURSE`, directories are also
            # included in the file list. These directories are only used for checking
            # whether destination (if exists) is symlink. They will not be copied.
            _remove_target_symlink(${_file} ${_dest}/${_name}/${_rel_path})
        else()
            _copy_if_different(${_file} ${_dest}/${_name}/${_rel_path})
        endif()
    endforeach()
endmacro()

foreach(_item ${src})
    if(NOT IS_ABSOLUTE ${_item})
        get_filename_component(_item ${_item} ABSOLUTE)
    endif()

    if(IS_DIRECTORY ${_item})
        _copy_dir(${_item})
    else()
        # if(${_item} MATCHES ".+\\*\\*")
        # string(REPLACE "**" "*" _item ${_item})
        # file(GLOB _files LIST_DIRECTORIES TRUE ${_item})
        # else()
        # file(GLOB _files ${_item})
        # endif()
        file(GLOB _files LIST_DIRECTORIES TRUE ${_item})

        foreach(_file ${_files})
            if(NOT IS_SYMLINK ${_file} AND IS_DIRECTORY ${_file})
                _copy_dir(${_file})
            else()
                get_filename_component(_name ${_file} NAME)
                _copy_if_different(${_file} ${_dest}/${_name})
            endif()
        endforeach()
    endif()
endforeach()

if(${_cnt} GREATER 0)
    message(STATUS "Copy ${_cnt} files in total.")
endif()