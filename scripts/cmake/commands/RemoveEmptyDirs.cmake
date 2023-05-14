# Usage:
# cmake
# -D dir=<dir>
# -P RemoveEmptyDirs.cmake

# remove all empty sub-directories

include(${CMAKE_CURRENT_LIST_DIR}/../ChorusKitUtils.cmake)

ck_check_defined(dir)

set(_search_dirs ${dir})

while(_search_dirs)
    list(POP_FRONT _search_dirs _curdir)
    file(GLOB _children RELATIVE ${_curdir} ${_curdir}/*)

    if(NOT _children)
        get_filename_component(_parent ${_curdir} DIRECTORY)

        # Add parent back
        list(FIND _search_dirs ${_parent} _idx)

        if(${_idx} EQUAL -1)
            list(APPEND _search_dirs ${_parent})
        endif()

        message(STATUS "Remove empty directory ${_curdir}")
        file(REMOVE_RECURSE ${_curdir})
        continue()
    endif()

    foreach(_child ${_children})
        set(_full_path ${_curdir}/${_child})

        if(IS_DIRECTORY ${_curdir}/${_child})
            list(APPEND _search_dirs ${_full_path})
        endif()
    endforeach()
endwhile()