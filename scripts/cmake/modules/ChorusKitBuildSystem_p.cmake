include_guard(DIRECTORY)

# ----------------------------------
# Internal parts
# ----------------------------------
macro(_ck_get_targets_recursive _targets _dir)
    get_property(_subdirs DIRECTORY ${_dir} PROPERTY SUBDIRECTORIES)

    foreach(_subdir ${_subdirs})
        _ck_get_targets_recursive(${_targets} ${_subdir})
    endforeach()

    get_property(_current_targets DIRECTORY ${_dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${_targets} ${_current_targets})
endmacro()

macro(_ck_list_prepend_prefix _list _prefix)
    foreach(_item ${ARGN})
        list(APPEND ${_list} ${_prefix}${_item})
    endforeach()
endmacro()

macro(_ck_list_remove_all _list1 _list2)
    foreach(_item ${${_list2}})
        list(REMOVE_ITEM ${_list1} ${_item})
    endforeach()
endmacro()

macro(_ck_list_add_flatly _list)
    set(_temp_list)
    file(GLOB _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()

macro(_ck_list_add_recursively _list)
    set(_temp_list)
    file(GLOB_RECURSE _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()

# Input: cxx source files
# Output: target ts files
function(_ck_add_lupdate_target _target)
    set(options CREATE_ONCE)
    set(oneValueArgs)
    set(multiValueArgs INPUT OUTPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LUPDATE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lupdate_deps ${_LUPDATE_DEPENDS})

    set(_my_sources ${_LUPDATE_INPUT})
    set(_my_tsfiles ${_LUPDATE_OUTPUT})

    add_custom_target(${_target} DEPENDS ${_lupdate_deps})
    get_target_property(_lupdate_exe "${Qt${QT_VERSION_MAJOR}_LUPDATE_EXECUTABLE}" IMPORTED_LOCATION)

    foreach(_ts_file ${_my_tsfiles})
        # make a list file to call lupdate on, so we don't make our commands too
        # long for some systems
        get_filename_component(_ts_name ${_ts_file} NAME)
        set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ts_name}_lst_file")
        set(_lst_file_srcs)

        foreach(_lst_file_src ${_my_sources})
            set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
        endforeach()

        get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)

        foreach(_pro_include ${_inc_DIRS})
            get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
            set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
        endforeach()

        file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")

        get_filename_component(_ts_abs ${_ts_file} ABSOLUTE)

        if(_LUPDATE_CREATE_ONCE AND NOT EXISTS ${_ts_abs})
            set(_options_filtered)

            foreach(_opt ${_LUPDATE_OPTIONS})
                if(_opt MATCHES ".*\\\$.*")
                    continue()
                endif()

                list(APPEND _options_filtered ${_opt})
            endforeach()

            message(STATUS "[INFO] Linguist update, generate ${_ts_name}")
            get_filename_component(_abs_file ${_ts_file} ABSOLUTE)
            get_filename_component(_dir ${_abs_file} DIRECTORY)
            make_directory(${_dir})
            execute_process(
                COMMAND ${_lupdate_exe} ${_options_filtered} "@${_ts_lst_file}" -ts ${_ts_file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE _null
            )
        endif()

        add_custom_command(
            TARGET ${_target}
            COMMAND ${_lupdate_exe}
            ARGS ${_LUPDATE_OPTIONS} "@${_ts_lst_file}" -ts ${_ts_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_my_sources}
            BYPRODUCTS ${_ts_lst_file} VERBATIM
        )
    endforeach()
endfunction()

# Input: ts files
# Output: list to append qm files
function(_ck_add_lrelease_target _target)
    set(options)
    set(oneValueArgs DESTINATION OUTPUT)
    set(multiValueArgs INPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LRELEASE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lrelease_files ${_LRELEASE_INPUT})
    set(_lrelease_deps ${_LRELEASE_DEPENDS})

    get_target_property(_lrelease_exe "${Qt${QT_VERSION_MAJOR}_LRELEASE_EXECUTABLE}" IMPORTED_LOCATION)

    set(_qm_files)

    foreach(_file ${_lrelease_files})
        get_filename_component(_abs_FILE ${_file} ABSOLUTE)
        get_filename_component(_qm_file ${_file} NAME)

        # everything before the last dot has to be considered the file name (including other dots)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${_qm_file})
        get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)

        if(output_location)
            set(_out_dir ${output_location})
        elseif(_LRELEASE_DESTINATION)
            set(_out_dir ${_LRELEASE_DESTINATION})
        else()
            set(_out_dir ${CMAKE_CURRENT_BINARY_DIR})
        endif()

        set(_qm_file "${_out_dir}/${FILE_NAME}.qm")

        add_custom_command(
            OUTPUT ${_qm_file}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_out_dir}
            COMMAND ${_lrelease_exe} ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${_qm_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_lrelease_files}
            VERBATIM
        )

        list(APPEND _qm_files ${_qm_file})
    endforeach()

    add_custom_target(${_target} ALL DEPENDS ${_lrelease_deps} ${_qm_files})

    if(_LRELEASE_OUTPUT)
        set(${_LRELEASE_OUTPUT} ${_qm_files} PARENT_SCOPE)
    endif()
endfunction()

# function(_ck_install_target _target)
# get_target_property()
# endfunction()

# function(_ck_install_attaches _src _dest _org_dir _new_dir)
# set(_scripts)
# list(APPEND _scripts "file(RELATIVE_PATH _rel_path \"${_org_dir}\" ${_dest})")
# list(APPEND _scripts "file(COPY \"${_src}\" DESTINATION \"${_new_dir}/\${_rel_path}\")")
# string(JOIN "\n" _script ${_scripts})
# install(CODE ${_script})
# endfunction()
function(_ck_configure_build_info_header _dir)
    # Configure build information header
    set(_git_branch "unknown")
    set(_git_hash "unknown")

    find_package(Git QUIET)

    if(GIT_FOUND)
        # message(STATUS "Git found: ${GIT_EXECUTABLE}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
            OUTPUT_VARIABLE _git_hash
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            ${REPOSITORY_ROOT_DIR}
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
            OUTPUT_VARIABLE _git_branch
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            ${REPOSITORY_ROOT_DIR}
        )
    else()
        message(STATUS "Git not found")
    endif()

    set(CHORUSKIT_BUILD_COMPILER_ID "${CMAKE_CXX_COMPILER_ID}")
    set(CHORUSKIT_BUILD_COMPILER_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
    set(CHORUSKIT_BUILD_COMPILER_ARCH "${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
    string(TIMESTAMP CHORUSKIT_BUILD_DATE_TIME "%Y/%m/%d %H:%M:%S")
    string(TIMESTAMP CHORUSKIT_BUILD_YEAR "%Y")
    set(CHORUSKIT_GIT_COMMIT_HASH "${_git_hash}")
    set(CHORUSKIT_GIT_BRANCH "${_git_branch}")

    configure_file(${CHORUSKIT_CMAKE_MODULES_DIR}/ChorusKitBuildInfo.h.in
        ${_dir}/ChorusKitBuildInfo.h
        @ONLY
    )
endfunction()
