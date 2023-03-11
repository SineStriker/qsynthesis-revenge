include_guard(DIRECTORY)

if(NOT CHORUSKIT_CMAKE_MODULES_DIR)
    set(CHORUSKIT_CMAKE_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})
endif()

#[[
Check if a variable is defined, throw error if not.

    ck_check_defined(var)
]] #
function(ck_check_defined _var)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "$\"{_var}\" is not defined")
    endif()
endfunction()

#[[
The macro works same as "option".

    ck_option(<name> <values...>)
]] #
macro(ck_option _name)
    if(NOT DEFINED ${_name})
        set(${_name} ${ARGN})
    endif()
endmacro()

#[[
Parse version and create seq vars with specified prefix.

    ck_parse_version(<prefix> <version>)
]] #
function(ck_parse_version _prefix _version)
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ ${_version})

    foreach(_i RANGE 1 4)
        if(${CMAKE_MATCH_COUNT} GREATER_EQUAL ${_i})
            set(_tmp ${CMAKE_MATCH_${_i}})
        else()
            set(_tmp 0)
        endif()

        set(${_prefix}_${_i} ${_tmp} PARENT_SCOPE)
    endforeach()
endfunction()

#[[
Get shorter version.

    ck_get_short_version(<VAR> <version> <count>)
]] #
function(ck_get_short_version _var _version _count)
    ck_parse_version(FUNC ${_version})

    set(_list)

    foreach(_i RANGE 1 ${_count})
        list(APPEND _list ${FUNC_${_i}})
    endforeach()

    string(JOIN "." _short_version ${_list})
    set(${_var} ${_short_version} PARENT_SCOPE)
endfunction()