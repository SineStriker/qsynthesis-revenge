include_guard(DIRECTORY)

#[[
Check if a variable is defined, throw error if not.

    ck_check_defined(var)
]] #
function(ck_check_defined _var)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "\"${_var}\" is not defined")
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

#[[
Tell if there are any generator expressions in the string.

    ck_get_short_version(<string> <output>)
]] #
function(ck_has_genex _str _out)
    string(GENEX_STRIP "${_str}" _no_genex)

    if("${_str}" STREQUAL "${_no_genex}")
        set(_res off)
    else()
        set(_res on)
    endif()

    set(${_out} ${_res} PARENT_SCOPE)
endfunction()

#[[
Get subdirectories' names or paths.

    ck_get_subdirs(<list>  
        [DIRECTORY dir]
        [EXCLUDE names...]
        [REGEX_INCLUDE exps...]
        [REGEX_EXLCUDE exps...]
        [RELATIVE path]
        [ABSOLUTE]
    )

    If `DIRECTORY` is not specified, consider `CMAKE_CURRENT_SOURCE_DIR`.
    If `RELATIVE` is specified, return paths evaluated as a relative path to it.
    If `ABSOLUTE` is specified, return absolute paths.
    If neither of them is specified, return names.
]] #
function(ck_get_subdirs _var)
    set(options ABSOLUTE)
    set(oneValueArgs DIRECTORY RELATIVE)
    set(multiValueArgs EXCLUDE REGEX_EXLCUDE)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(FUNC_DIRECTORY)
        get_filename_component(_dir ${FUNC_DIRECTORY} ABSOLUTE)
    else()
        set(_dir ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    file(GLOB _subdirs LIST_DIRECTORIES true RELATIVE ${_dir} "${_dir}/*")

    if(FUNC_EXCLUDE)
        foreach(_exclude_dir ${FUNC_EXCLUDE})
            list(REMOVE_ITEM _subdirs ${_exclude_dir})
        endforeach()
    endif()

    if(FUNC_REGEX_INCLUDE)
        foreach(_exp ${FUNC_REGEX_INCLUDE})
            list(FILTER _subdirs INCLUDE REGEX ${_exp})
        endforeach()
    endif()

    if(FUNC_REGEX_EXCLUDE)
        foreach(_exp ${FUNC_REGEX_EXCLUDE})
            list(FILTER _subdirs EXCLUDE REGEX ${_exp})
        endforeach()
    endif()

    set(_res)

    if(FUNC_RELATIVE)
        get_filename_component(_relative ${FUNC_RELATIVE} ABSOLUTE)
    else()
        set(_relative)
    endif()

    foreach(_sub ${_subdirs})
        if(IS_DIRECTORY ${_dir}/${_sub})
            if(FUNC_ABSOLUTE)
                list(APPEND _res ${_dir}/${_sub})
            elseif(_relative)
                file(RELATIVE_PATH _rel_path ${_relative} ${_dir}/${_sub})
                list(APPEND _res ${_rel_path})
            else()
                list(APPEND _res ${_sub})
            endif()
        endif()
    endforeach()

    set(${_var} ${_res} PARENT_SCOPE)
endfunction()

#[[
Find Qt modules by calling `find_package`.

    ck_find_qt_module(<modules...>)
]] #
macro(ck_find_qt_module)
    foreach(_module ${ARGV})
        find_package(QT NAMES Qt6 Qt5 COMPONENTS ${_module} REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${_module} REQUIRED)
    endforeach()
endmacro()

#[[
Add Qt modules to the specified list.

    ck_add_qt_module(<list> <modules...>)
]] #
macro(ck_add_qt_module _list)
    foreach(_module ${ARGN})
        ck_find_qt_module(${_module})
        list(APPEND ${_list} Qt${QT_VERSION_MAJOR}::${_module})
    endforeach()
endmacro()

#[[
Add Qt private include directories to the specified list.

    ck_add_qt_private_inc(<list> <modules...>)
]] #
macro(ck_add_qt_private_inc _list)
    foreach(_module ${ARGN})
        list(APPEND ${_list} ${Qt${QT_VERSION_MAJOR}${_module}_PRIVATE_INCLUDE_DIRS})
    endforeach()
endmacro()