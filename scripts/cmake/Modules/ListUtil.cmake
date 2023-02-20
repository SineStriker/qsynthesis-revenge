include_guard(DIRECTORY)

#[[
    add_files <var> 
            PATTERNS patterns... 
            [CURRENT] 
            [CURRENT_RECURSE] 
            [SUBDIRS_EXCEPT dirs...]
            [DIRECTORIES dirs...] 
            [CLEAR]

    args:
        var:                list to accept files
        patterns:           file patterns, e.g. *.h *.cpp *.hpp *.cc

    command:
        DIRECTORIES:        add other directories
        SUBDIRS:            add all subdirectories
        SUBDIRS_EXCEPT:     add all subdirectories other than following names (No need to set SUBDIRS)

    flags:
        CURRENT:            add matched files in current directory
        CURRENT_RECURSE:    add matched files in current directory and all subdirectories recursively
        CLEAR:              clear the list firstly

    usage:
        search files of given patterns and append them into a list
#]]
function(add_files _out)
    set(options CLEAR CURRENT CURRENT_RECURSE SUBDIRS)
    set(oneValueArgs)
    set(multiValueArgs SUBDIRS_EXCEPT DIRECTORIES PATTERNS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_temp_src)

    if(FUNC_CLEAR)
        set(_src)
    else()
        set(_src ${${_out}})
    endif()

    # Add current dir
    if(FUNC_CURRENT_RECURSE)
        foreach(_pat ${FUNC_PATTERNS})
            list_add_recursively(_temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${_pat})
        endforeach()
    elseif(FUNC_CURRENT)
        foreach(_pat ${FUNC_PATTERNS})
            list_add_flatly(_temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${_pat})
        endforeach()
    endif()

    # Add sub dirs
    if(FUNC_SUBDIRS OR FUNC_SUBDIRS_EXCEPT)
        if(FUNC_SUBDIRS_EXCEPT)
            get_subdirs(_subdirs EXCLUDE ${FUNC_SUBDIRS_EXCEPT})
        else()
            get_subdirs(_subdirs)
        endif()

        foreach(_dir ${_subdirs})
            foreach(_pat ${FUNC_PATTERNS})
                list_add_flatly(_temp_src ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/${_pat})
            endforeach()
        endforeach()
    endif()

    # Add other dirs recursively
    foreach(_dir ${FUNC_DIRECTORIES})
        foreach(_pat ${FUNC_PATTERNS})
            list_add_recursively(_temp_src ${_dir}/${_pat})
        endforeach()
    endforeach()

    list(APPEND _src ${_temp_src})
    set(${_out} ${_src} PARENT_SCOPE)
endfunction()

#[[
    get_subdirs <var>  
            [DIRECTORY dir] 
            [EXCLUDE names...] 
            [REGEX_EXLCUDE exps...]
            [ABSOLUTE]

    args:
        var:                list to accept files
        patterns:           file patterns, e.g. *.h *.cpp *.hpp *.cc

    command:
        DIRECTORY:          add other directories
        EXCLUDE:            exclude subdirectories of following names
        REGEX_EXLCUDE:      exclude subdirectories matching following expressions
        RELATIVE:           return relative paths to the following path

    flags:
        ABSOLUTE:           return absolute paths

    usage:
        get subdirectories' names or paths
]] #
function(get_subdirs _out)
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

    if(FUNC_REGEX_EXCLUDE)
        foreach(_exp ${FUNC_REGEX_EXCLUDE})
            list(FILTER _subdirs EXCLUDE REGEX ${_exp})
        endforeach()
    endif()

    set(_res)

    foreach(_sub ${_subdirs})
        if(IS_DIRECTORY ${_dir}/${_sub})
            if(FUNC_ABSOLUTE)
                list(APPEND _res ${_dir}/${_sub})
            elseif(FUNC_RELATIVE)
                get_filename_component(_path ${FUNC_RELATIVE} ABSOLUTE)
                file(RELATIVE_PATH _rel_path ${_path} ${_dir}/${_sub})
                list(APPEND _res ${_rel_path})
            else()
                list(APPEND _res ${_sub})
            endif()
        endif()
    endforeach()

    set(${_out} ${_res} PARENT_SCOPE)
endfunction()

macro(list_append_with_prefix _list _prefix)
    foreach(_item ${ARGN})
        list(APPEND ${_list} ${_prefix}${_item})
    endforeach()
endmacro()

macro(list_remove_all _list1 _list2)
    foreach(_item ${${_list2}})
        list(REMOVE_ITEM ${_list1} ${_item})
    endforeach()
endmacro()

macro(list_add_flatly _list)
    set(_temp_list)
    file(GLOB _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()

macro(list_add_recursively _list)
    set(_temp_list)
    file(GLOB_RECURSE _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()