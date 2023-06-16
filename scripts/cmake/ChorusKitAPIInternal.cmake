include_guard(GLOBAL)

include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitAPIConfig.cmake)

# Find python, git, qmake for further usage
if(CHORUSKIT_REPOSITORY)
    if(WIN32)
        set(CK_SHARED_LIBRARY_PATTERN "*.dll")
    elseif(APPLE)
        set(CK_SHARED_LIBRARY_PATTERN "*.dylib")
    else()
        set(CK_SHARED_LIBRARY_PATTERN "*.so*")
    endif()

    # Git
    find_package(Git QUIET)

    if(Git_FOUND)
        message(STATUS "Git found: ${GIT_EXECUTABLE}")
    else()
        message(WARNING "Git not found")
    endif()

    # Python
    find_package(Python QUIET)

    if(Python_FOUND)
        message(STATUS "Python found: ${Python_EXECUTABLE}")
    else()
        message(WARNING "Python not found")
    endif()

    # Find Qt tools
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core QUIET)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core QUIET)

    if(TARGET Qt${QT_VERSION_MAJOR}::qmake)
        if(NOT DEFINED QT_QMAKE_EXECUTABLE)
            get_target_property(QT_QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
        endif()

        if(EXISTS "${QT_QMAKE_EXECUTABLE}")
            message(STATUS "Qmake found: ${QT_QMAKE_EXECUTABLE}")
        else()
            set(QT_QMAKE_EXECUTABLE)
            message(WARNING "Qmake not found")
        endif()
    else()
        message(WARNING "QtCore component not found")
    endif()
endif()

# ----------------------------------
# ChorusKit Private API
# ----------------------------------
macro(_ck_set_cmake_autoxxx _val)
    set(CMAKE_AUTOMOC ${_val})
    set(CMAKE_AUTOUIC ${_val})
    set(CMAKE_AUTORCC ${_val})
endmacro()

function(_ck_parse_namespace _nested _key _val)
    string(REGEX MATCH "(.+)__(.+)" _ ${_nested})

    if(${CMAKE_MATCH_COUNT} EQUAL 2)
        set(${_key} ${CMAKE_MATCH_1} PARENT_SCOPE)
        set(${_val} ${CMAKE_MATCH_2} PARENT_SCOPE)
    endif()
endfunction()

function(_ck_convert_target_to_alias _out)
    set(_res)

    foreach(_item ${ARGN})
        set(_key)
        set(_val)
        _ck_parse_namespace(${_item} _key _val)

        if(_key AND _val)
            list(APPEND _res ${_key}::${_val})
            continue()
        endif()

        list(APPEND _res ${_item})
    endforeach()

    set(${_out} ${_res} PARENT_SCOPE)
endfunction()

function(_ck_check_shared_library _target _out)
    set(_res off)
    get_target_property(_type ${_target} TYPE)

    if(${_type} STREQUAL SHARED_LIBRARY)
        set(_res on)
    endif()

    set(${_out} ${_res} PARENT_SCOPE)
endfunction()

function(_ck_resolve_src_dest _args _result _error)
    set(_src)
    set(_dest)
    set(_status NONE) # NONE, SRC, DEST
    set(_count 0)

    set(_list)

    foreach(_item ${_args})
        if(${_item} STREQUAL SRC)
            if(${_status} STREQUAL NONE)
                set(_src)
                set(_status SRC)
            elseif(${_status} STREQUAL DEST)
                set(${_error} "missing directory name after DEST!" PARENT_SCOPE)
                return()
            else()
                set(${_error} "missing source files after SRC!" PARENT_SCOPE)
                return()
            endif()
        elseif(${_item} STREQUAL DEST)
            if(${_status} STREQUAL SRC)
                set(_status DEST)
            elseif(${_status} STREQUAL DEST)
                set(${_error} "missing directory name after DEST!" PARENT_SCOPE)
                return()
            else()
                set(${_error} "no source files specified for DEST!" PARENT_SCOPE)
                return()
            endif()
        else()
            if(${_status} STREQUAL NONE)
                set(${_error} "missing SRC or DEST token!" PARENT_SCOPE)
                return()
            elseif(${_status} STREQUAL DEST)
                if(NOT _src)
                    set(${_error} "no source files specified for DEST!" PARENT_SCOPE)
                    return()
                endif()

                set(_status NONE)
                math(EXPR _count "${_count} + 1")

                string(JOIN "\\;" _src_str ${_src})
                list(APPEND _list "${_src_str}\\;${_item}")
            else()
                get_filename_component(_path ${_item} ABSOLUTE)
                list(APPEND _src ${_path})
            endif()
        endif()
    endforeach()

    if(${_status} STREQUAL SRC)
        set(${_error} "missing DEST after source files!" PARENT_SCOPE)
        return()
    elseif(${_status} STREQUAL DEST)
        set(${_error} "missing directory name after DEST!" PARENT_SCOPE)
        return()
    elseif(${_count} STREQUAL 0)
        set(${_error} "no files specified!" PARENT_SCOPE)
        return()
    endif()

    set(${_result} "${_list}" PARENT_SCOPE)
endfunction()

function(_ck_add_copy_command _target _base_dir _src _dest)
    set(options SKIP_BUILD SKIT_INSTALL)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    ck_has_genex(${_dest} _has_genex)

    if(_has_genex)
        set(_path ${_dest})
    else()
        if(IS_ABSOLUTE ${_dest})
            set(_path ${_dest})
        else()
            set(_path ${_base_dir}/${_dest})
        endif()
    endif()

    foreach(_src_item ${_src})
        get_filename_component(_full_path ${_src_item} ABSOLUTE)

        # Add a post target to handle unexpected delete
        if(NOT FUNC_SKIP_BUILD)
            add_custom_command(TARGET ${_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -D "src=${_full_path}"
                -D "dest=${_path}"
                -P "${CK_CMAKE_SCRIPTS_DIR}/CopyIfDifferent.cmake"
            )
        endif()

        if(NOT FUNC_SKIP_INSTALL)
            install(CODE "
                file(RELATIVE_PATH _rel_path \"${CK_MAIN_OUTPUT_PATH}\" \"${_path}\")
                execute_process(
                    COMMAND \"${CMAKE_COMMAND}\"
                    -D \"src=${_full_path}\"
                    -D \"dest=${CMAKE_INSTALL_PREFIX}/\${_rel_path}\"
                    -P \"${CK_CMAKE_SCRIPTS_DIR}/CopyIfDifferent.cmake\"
                    COMMAND_ERROR_IS_FATAL ANY
                )
            ")
        endif()
    endforeach()
endfunction()

function(_ck_append_define_line _key _val _out)
    string(LENGTH ${_key} _len)
    math(EXPR _cnt "40-${_len}")
    string(REPEAT " " ${_cnt} _spaces)
    list(APPEND ${_out} "#define ${_key}${_spaces}\"${_val}\"")
    set(${_out} ${${_out}} PARENT_SCOPE)
endfunction()

function(_ck_generate_config_header _def_list _file)
    set(_options)
    set(_values)

    foreach(_item ${_def_list})
        string(REGEX MATCH "(.+)=(.+)" _ ${_item})

        if(${CMAKE_MATCH_COUNT} EQUAL 2)
            set(_key ${CMAKE_MATCH_1})
            set(_val ${CMAKE_MATCH_2})
            _ck_append_define_line(${_key} ${_val} _values)
            continue()
        endif()

        list(APPEND _options "#define ${_item}")
    endforeach()

    string(JOIN "\n" _content "#pragma once" "" ${_options} "" ${_values} "")
    file(GENERATE OUTPUT ${_file} CONTENT ${_content})
endfunction()

function(_ck_configure_build_info_header _file)
    # Configure build information header
    set(_git_branch "unknown")
    set(_git_hash "unknown")

    if(Git_FOUND)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
            OUTPUT_VARIABLE _git_hash
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            ${CK_PROJECT_ROOT_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )

        execute_process(
            COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
            OUTPUT_VARIABLE _git_branch
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
            WORKING_DIRECTORY
            ${CK_PROJECT_ROOT_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    set(_compiler_name unknown)

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        set(_compiler_name "Clang")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(_compiler_name "GCC")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(_compiler_name "MSVC")
    elseif(CMAKE_CXX_COMPILER_ID)
        set(_compiler_name ${CMAKE_CXX_COMPILER_ID})
    endif()

    set(_compiler_arch ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})

    if(NOT _compiler_arch)
        string(TOLOWER ${CMAKE_HOST_SYSTEM_PROCESSOR} _compiler_arch)
    endif()

    set(_compiler_version ${CMAKE_CXX_COMPILER_VERSION})

    if(NOT _compiler_version)
        set(_compiler_version 0)
    endif()

    string(TIMESTAMP _build_time "%Y/%m/%d %H:%M:%S")
    string(TIMESTAMP _build_year "%Y")

    set(_values)
    _ck_append_define_line(CHORUSKIT_BUILD_COMPILER_ID ${_compiler_name} _values)
    _ck_append_define_line(CHORUSKIT_BUILD_COMPILER_VERSION ${_compiler_version} _values)
    _ck_append_define_line(CHORUSKIT_BUILD_COMPILER_ARCH ${_compiler_arch} _values)
    _ck_append_define_line(CHORUSKIT_BUILD_DATE_TIME ${_build_time} _values)
    _ck_append_define_line(CHORUSKIT_BUILD_YEAR ${_build_year} _values)
    _ck_append_define_line(CHORUSKIT_GIT_COMMIT_HASH ${_git_hash} _values)
    _ck_append_define_line(CHORUSKIT_GIT_BRANCH ${_git_branch} _values)

    string(JOIN "\n" _content "#pragma once" "" ${_values} "")
    file(GENERATE OUTPUT ${_file} CONTENT ${_content})
endfunction()

function(_ck_configure_plugin_desc _file)
    set(options ALL_FILES ALL_SUBDIRS)
    set(oneValueArgs NAME)
    set(multiValueArgs SUBDIRS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _ck_set_value(_name FUNC_NAME ${PROJECT_NAME})

    set(_content "{\n    \"name\": \"${_name}\"")

    if(FUNC_ALL_FILES)
        set(_content "${_content},\n    \"allFiles\": true")
    endif()

    if(FUNC_ALL_SUBDIRS)
        set(_content "${_content},\n    \"allSubdirs\": true")
    endif()

    if(FUNC_SUBDIRS)
        string(JOIN "\",\n        \"" _json_arr_str ${FUNC_SUBDIRS})
        set(_content "${_content},\n    \"subdirs\": [\n        \"${_json_arr_str}\"\n    ]")
    endif()

    set(_content "${_content}\n}")

    file(GENERATE OUTPUT ${_file} CONTENT ${_content})
endfunction()

macro(_ck_set_value _key _maybe_value _default)
    if(${_maybe_value})
        set(${_key} ${${_maybe_value}})
    else()
        set(${_key} ${_default})
    endif()
endmacro()

function(_ck_try_set_output_name _target _name)
    get_target_property(_org ${_target} OUTPUT_NAME)

    if(NOT _org)
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_name})
    endif()
endfunction()

function(_ck_add_library_internal _target)
    set(options SHARED)
    set(oneValueArgs NAME MACRO_PREFIX LIBRARY_MACRO TYPE_MACRO)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(FUNC_MACRO_PREFIX)
        set(_prefix ${FUNC_MACRO_PREFIX})
    elseif(FUNC_NAME)
        string(TOUPPER ${FUNC_NAME} _prefix)
    else()
        string(TOUPPER ${_target} _prefix)
    endif()

    if(FUNC_LIBRARY_MACRO)
        set(_library_macro ${FUNC_LIBRARY_MACRO})
    else()
        set(_library_macro ${_prefix}_LIBRARY)
    endif()

    if(FUNC_TYPE_MACRO)
        set(_type_macro ${FUNC_TYPE_MACRO})
    else()
        if(FUNC_SHARED)
            set(_type_macro ${_prefix}_SHARED)
        else()
            set(_type_macro ${_prefix}_STATIC)
        endif()
    endif()

    if(FUNC_SHARED)
        add_library(${_target} SHARED)
    else()
        add_library(${_target} STATIC)
    endif()

    if(FUNC_NAME)
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${FUNC_NAME})
    endif()

    target_compile_definitions(${_target} PUBLIC ${_type_macro})
    target_compile_definitions(${_target} PRIVATE ${_library_macro})
endfunction()

function(_ck_attach_win_rc_file _target)
    set(options MANIFEST)
    set(oneValueArgs NAME VERSION DESCRIPTION COPYRIGHT ICO)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _ck_set_value(_app_name FUNC_NAME ${_target})
    _ck_set_value(_app_version FUNC_VERSION "${PROJECT_VERSION}")
    _ck_set_value(_app_desc FUNC_DESCRIPTION ${_app_name})
    _ck_set_value(_app_copyright FUNC_COPYRIGHT ${_target})

    ck_parse_version(_app_version ${_app_version})

    if(FUNC_ICO)
        set(RC_ICON_COMMENT)
        get_filename_component(RC_ICON_PATH ${FUNC_ICO} ABSOLUTE)
    else()
        set(RC_ICON_COMMENT "//")
        set(RC_ICON_PATH)
    endif()

    set(RC_VERSION ${_app_version_1},${_app_version_2},${_app_version_3},${_app_version_4})
    set(RC_APPLICATION_NAME ${_app_name})
    set(RC_VERSION_STRING ${_app_version})
    set(RC_DESCRIPTION ${_app_desc})
    set(RC_COPYRIGHT ${_app_copyright})

    set(_rc_path ${CMAKE_CURRENT_BINARY_DIR}/${_target}.rc)
    configure_file(${CK_CMAKE_MODULES_DIR}/WinResource.rc.in ${_rc_path} @ONLY)
    target_sources(${_target} PRIVATE ${_rc_path})

    if(FUNC_MANIFEST)
        set(MANIFEST_VERSION ${_app_version})
        set(MANIFEST_IDENTIFIER ${_app_name})
        set(MANIFEST_DESCRIPTION ${_app_desc})

        set(_manifest_path ${CMAKE_CURRENT_BINARY_DIR}/${_target}.manifest)
        configure_file(${CK_CMAKE_MODULES_DIR}/WinManifest.manifest.in ${_manifest_path} @ONLY)
        target_sources(${_target} PRIVATE ${_manifest_path})
    endif()

    if(FUNC_NAME)
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${FUNC_NAME})
    endif()
endfunction()

function(_ck_attach_mac_bundle _target)
    set(options)
    set(oneValueArgs NAME VERSION DESCRIPTION COPYRIGHT ICNS)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _ck_set_value(_app_name FUNC_NAME ${_target})
    _ck_set_value(_app_version FUNC_VERSION "${PROJECT_VERSION}")
    _ck_set_value(_app_desc FUNC_DESCRIPTION ${_app_name})
    _ck_set_value(_app_copyright FUNC_COPYRIGHT ${_target})

    ck_parse_version(_app_version ${_app_version})

    # configure mac plist
    set_target_properties(${_target} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME ${_app_name}
        MACOSX_BUNDLE_EXECUTABLE_NAME ${_app_name}
        MACOSX_BUNDLE_INFO_STRING ${_app_desc}
        MACOSX_BUNDLE_GUI_IDENTIFIER ${_app_name}
        MACOSX_BUNDLE_BUNDLE_VERSION ${_app_version}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${_app_version_1}.${_app_version_2}
        MACOSX_BUNDLE_COPYRIGHT ${_app_copyright}
    )

    if(FUNC_ICNS)
        # And this part tells CMake where to find and install the file itself
        set_source_files_properties(${FUNC_ICNS} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
        )

        # NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
        # the property added to Info.plist
        get_filename_component(_icns_name ${FUNC_ICNS} NAME)

        # configure mac plist
        set_target_properties(${_target} PROPERTIES
            MACOSX_BUNDLE_ICON_FILE ${_icns_name}
        )

        # ICNS icon MUST be added to executable's sources list, for some reason
        # Only apple can do
        target_sources(${_target} PRIVATE ${FUNC_ICNS})
    endif()

    if(FUNC_NAME)
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${FUNC_NAME})
    endif()
endfunction()

function(_ck_create_win_shortcut _target _dir)
    set(options)
    set(oneValueArgs OUTPUT_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _ck_set_value(_output_name FUNC_OUTPUT_NAME $<TARGET_FILE_BASE_NAME:${_target}>)

    string(RANDOM LENGTH ${CK_CMAKE_RANDOM_LENGTH} _rand)
    set(_vbs_name ${CMAKE_CURRENT_BINARY_DIR}/${_target}_shortcut.vbs)
    set(_vbs_temp ${_vbs_name}.in)

    set(_lnk_path "${_dir}/${_output_name}.lnk")

    set(SHORTCUT_PATH ${_lnk_path})
    set(SHORTCUT_TARGET_PATH $<TARGET_FILE:${_target}>)
    set(SHORTCUT_WORKING_DIRECOTRY $<TARGET_FILE_DIR:${_target}>)
    set(SHORTCUT_DESCRIPTION $<TARGET_FILE_BASE_NAME:${_target}>)
    set(SHORTCUT_ICON_LOCATION $<TARGET_FILE:${_target}>)

    configure_file(
        ${CK_CMAKE_MODULES_DIR}/WinCreateShortcut.vbs.in
        ${_vbs_temp}
        @ONLY
    )
    file(GENERATE OUTPUT ${_vbs_name} INPUT ${_vbs_temp})

    add_custom_command(
        TARGET ${_target} POST_BUILD
        COMMAND cscript ${_vbs_name}
        BYPRODUCTS ${_lnk_path}
    )
endfunction()

function(_ck_set_win32_executable_type _target)
    # Set windows application type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(NOT CK_CONSOLE_ON_DEBUG)
            set_target_properties(${_target} PROPERTIES
                WIN32_EXECUTABLE TRUE
            )
        endif()
    else()
        if(NOT CK_CONSOLE_ON_RELEASE)
            set_target_properties(${_target} PROPERTIES
                WIN32_EXECUTABLE TRUE
            )
        endif()
    endif()
endfunction()

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
                ck_has_genex(${_opt} _has_genex)

                if(_has_genex)
                    continue()
                endif()

                list(APPEND _options_filtered ${_opt})
            endforeach()

            message(STATUS "Linguist update: Generate ${_ts_name}")
            get_filename_component(_abs_file ${_ts_file} ABSOLUTE)
            get_filename_component(_dir ${_abs_file} DIRECTORY)
            make_directory(${_dir})
            execute_process(
                COMMAND ${_lupdate_exe} ${_options_filtered} "@${_ts_lst_file}" -ts ${_ts_file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE _null
                COMMAND_ERROR_IS_FATAL ANY
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

function(_ck_dist_shared_for_application _item)
    # Add post build event of copying shared files
    add_custom_command(TARGET ${_item} POST_BUILD
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ChorusKit_CopySharedFiles
    )

    if(APPLE)
        # Add post build event of making soft link for shared files
        set(_base_dir $<TARGET_BUNDLE_DIR:${_item}>/Contents)
        add_custom_command(TARGET ${_item} POST_BUILD

            # Make directories if not exist
            COMMAND mkdir -p ${_base_dir}/Frameworks
            COMMAND mkdir -p ${_base_dir}/Resources

            # Remove previous links
            COMMAND find ${_base_dir}/Frameworks -type l -delete
            COMMAND find ${_base_dir}/Resources -type l -delete

            # Make new links
            COMMAND ln -s ${CK_GLOBAL_LIBRARY_OUTPUT_PATH}/* ${_base_dir}/Frameworks
            COMMAND ln -s ${CK_GLOBAL_SHARE_OUTPUT_PATH}/* ${_base_dir}/Resources
        )
    endif()
endfunction()

function(_ck_deploy_qt_library)
    if(NOT Python_EXECUTABLE OR NOT QT_QMAKE_EXECUTABLE)
        return()
    endif()

    if(WIN32)
        set(_lib_dir ${CK_INSTALL_RUNTIME_OUTPUT_PATH})
    elseif(APPLE)
        set(_lib_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/Qt)
    else()
        set(_lib_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/Qt/lib)
    endif()

    set(_plugins_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/Qt/plugins)

    # Join target command
    set(_targets_cmd)
    get_target_property(_deploy_targets ChorusKit_Metadata DEPLOY_TARGETS)

    foreach(_target ${_deploy_targets})
        set(_targets_cmd "${_targets_cmd}\"$<TARGET_FILE:${_target}>\" ")
    endforeach()

    _ck_convert_target_to_alias(_alias_targets ${_deploy_targets})

    set(_script "${CK_PYTHON_SCRIPTS_DIR}/deployqt.py")
    install(CODE "
        message(STATUS \"Deploying: Run ${_script}\")
        message(STATUS \"Targets: ${_alias_targets}\")
        execute_process(
            COMMAND \"${Python_EXECUTABLE}\" \"${_script}\"
            --qmake \"${QT_QMAKE_EXECUTABLE}\"
            --libdir \"${_lib_dir}\"
            --plugindir \"${_plugins_dir}\"
            ${_targets_cmd}
            WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\"
            COMMAND_ERROR_IS_FATAL ANY
        )")
endfunction()