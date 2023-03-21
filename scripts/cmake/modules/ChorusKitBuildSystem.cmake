include_guard(DIRECTORY)
include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitGlobal.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitUtils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitBuildSystem_p.cmake)

#[[
Initalize ChorusKitApi global settings.

    ck_init_buildsystem(
        [OUTPUT_DIR dir]
    )
]] #
function(ck_init_buildsystem)
    set(options)
    set(oneValueArgs OUTPUT_DIR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Check operating system
    set(_linux OFF)
    set(_name)

    if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
        set(_linux ON)
        set(_name Linux)
    elseif(WIN32)
        set(_name Windows)
    elseif(APPLE)
        set(_name Macintosh)
    else()
        message(FATAL_ERROR "Unsupported System !!!")
    endif()

    message(STATUS "[INFO] Current System is ${_name}")

    set(LINUX ${_linux} PARENT_SCOPE)

    # Set CMAKE_XXX_OUTPUT_DIRECTORY
    if(FUNC_OUTPUT_DIR)
        set(_out_dir ${FUNC_OUTPUT_DIR})
    else()
        set(_out_dir ${CMAKE_BINARY_DIR}/out-${CMAKE_HOST_SYSTEM_NAME}-${CMAKE_BUILD_TYPE})
    endif()

    if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${_out_dir}/bin PARENT_SCOPE)
    endif()

    if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${_out_dir}/lib PARENT_SCOPE)
    endif()

    if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${_out_dir}/lib PARENT_SCOPE)
    endif()

    add_custom_target(ChorusKit_UpdateTranslations)
    add_custom_target(ChorusKit_ReleaseTranslations)
    add_custom_target(ChorusKit_CopyResources)
endfunction()

#[[
Add target to copy vcpkg dependencies.

    ck_init_vcpkg(
        [VCPKG_DIR  dir]
    )
]] #
function(ck_init_vcpkg _vcpkg_dir _vcpkg_triplet)
    # Deploy vcpkg dependencies
    set(_vcpkg_installed_dir ${_vcpkg_dir}/installed)

    if(${CMAKE_BUILD_TYPE} MATCHES "Debug|Dbg")
        set(_vcpkg_triplet_suffix "/debug")
    else()
        set(_vcpkg_triplet_suffix)
    endif()

    if(WIN32)
        set(_bin_dir ${_vcpkg_installed_dir}/${_vcpkg_triplet}${_vcpkg_triplet_suffix}/bin)
        set(_runtime_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else()
        set(_bin_dir ${_vcpkg_installed_dir}/${_vcpkg_triplet}${_vcpkg_triplet_suffix}/lib)
        set(_runtime_output_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    endif()

    set(_prebuilt_dlls)
    file(GLOB _dlls ${_bin_dir}/${CHORUSKIT_SHARED_LIBRARY_PATTERN})

    foreach(_dll ${_dlls})
        get_filename_component(_name ${_dll} NAME)
        set(_out_dll ${_runtime_output_dir}/${_name})

        add_custom_command(
            OUTPUT ${_out_dll}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_runtime_output_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_dll} ${_runtime_output_dir}
        )

        list(APPEND _prebuilt_dlls ${_out_dll})
    endforeach()

    add_custom_target(ChorusKit_SetupVcpkgDeps ALL DEPENDS ${_prebuilt_dlls})
endfunction()

#[[
Collect targets of given types recursively in a directory.

    ck_collect_targets(<list> [DIR directory]
                    [EXECUTABLE] [SHARED] [STATIC] [UTILITY])

    If one or more types are specified, return targets matching the types.
    If no type is specified, return all targets.
]] #
function(ck_collect_targets _var)
    set(options EXECUTABLE SHARED STATIC UTILITY)
    set(oneValueArgs DIR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(FUNC_DIR)
        set(_dir ${FUNC_DIR})
    else()
        set(_dir ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    set(_tmp_targets)
    set(_targets)

    # Get targets
    _ck_get_targets_recursive(_tmp_targets ${_dir})

    if(NOT FUNC_EXECUTABLE AND NOT FUNC_SHARED AND NOT FUNC_STATIC AND NOT FUNC_UTILITY)
        set(_targets ${_tmp_targets})
    else()
        # Filter targets
        foreach(_item ${_tmp_targets})
            get_target_property(_type ${_item} TYPE)

            if(${_type} STREQUAL "EXECUTABLE")
                if(FUNC_EXECUTABLE)
                    list(APPEND _targets ${_item})
                endif()
            elseif(${_type} STREQUAL "SHARED_LIBRARY")
                if(FUNC_SHARED)
                    list(APPEND _targets ${_item})
                endif()
            elseif(${_type} STREQUAL "STATIC_LIBRARY")
                if(FUNC_STATIC)
                    list(APPEND _targets ${_item})
                endif()
            elseif(${_type} STREQUAL "UTILITY")
                if(FUNC_UTILITY)
                    list(APPEND _targets ${_item})
                endif()
            endif()
        endforeach()
    endif()

    set(${_var} ${_targets} PARENT_SCOPE)
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

#[[
Get subdirectories' names or paths.

    ck_get_subdirs(<list>  
            [DIRECTORY dir]
            [EXCLUDE names...]
            [REGEX_INCLUDE exps...]
            [REGEX_EXLCUDE exps...]
            [RELATIVE path] [ABSOLUTE])

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
Add files with specified patterns to list.

    ck_add_files(<list> PATTERNS <patterns...>
        [CURRENT] [SUBDIRS] [ALLDIRS] [DIRS dirs]
        [FILTER_PLATFORM]
    )

    Directory arguments or options:
        CURRENT: consider only current directory
        SUBDIRS: consider all subdirectories recursively
        ALLDIRS: consider both `CURRENT` and `SUBDIRS`
        DIRS: consider extra directories recursively
    
    Filter options:
        FILTER_PLATFORM: exclude files like `*_win.xxx` on Unix aand `*_unix.xxx` on Windows
]] #
function(ck_add_files _var)
    set(options CURRENT SUBDIRS ALLDIRS)
    set(oneValueArgs)
    set(multiValueArgs PATTERNS DIRS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_src)

    # Add current dir
    if(FUNC_CURRENT OR FUNC_ALLDIRS)
        set(_tmp_patterns)

        foreach(_pat ${FUNC_PATTERNS})
            list(APPEND _tmp_patterns ${CMAKE_CURRENT_SOURCE_DIR}/${_pat})
        endforeach()

        _ck_list_add_flatly(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endif()

    # Add sub dirs
    if(FUNC_SUBDIRS OR FUNC_ALLDIRS)
        ck_get_subdirs(_subdirs ABSOLUTE)
        set(_tmp_patterns)

        foreach(_subdir ${_subdirs})
            foreach(_pat ${FUNC_PATTERNS})
                list(APPEND _tmp_patterns ${_subdir}/${_pat})
            endforeach()
        endforeach()

        _ck_list_add_recursively(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endif()

    # Add other dirs recursively
    foreach(_dir ${FUNC_DIRS})
        set(_tmp_patterns)

        foreach(_pat ${FUNC_PATTERNS})
            list(APPEND _tmp_patterns ${_dir}/${_pat})
        endforeach()

        _ck_list_add_recursively(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endforeach()

    if(FUNC_FILTER_PLATFORM)
        if(WIN32)
            list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.+")
        elseif(APPLE)
            list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.+")
        else()
            list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.+")
        endif()
    endif()

    set(${_var} ${_src} PARENT_SCOPE)
endfunction()

#[[
CMake target commands wrapper to add sources, links, includes.

    ck_target_components(<target>
        [SOURCES files...]

        [LINKS            libs...]
        [LINKS_PRIVATE    libs...]

        [INCLUDES         dirs...]
        [INCLUDES_PRIVATE dirs...]
        
        [INCLUDES_BUILD   dirs...]
        [INCLUDES_INSTALL dirs...]

        [DEFINES          defs...]
        [DEFINES_PRIVATE  defs...]

        [CCFLAGS          flags...]
        [CCFLAGS_PRIVATE  flags...]

        [QT_LINKS             modules...]
        [QT_LINKS_PRIVATE     modules...]
        [QT_INCLUDES_PRIVATE  modules...]

        [INCLUDE_CURRENT]
        [INCLUDE_SUBDIRS]

        [INCLUDE_CURRENT_PRIVATE]
        [INCLUDE_SUBDIRS_PRIVATE]
    )
]] #
function(ck_target_components _target)
    set(options INCLUDE_CURRENT INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS INCLUDE_SUBDIRS_PRIVATE)
    set(oneValueArgs)
    set(multiValueArgs SOURCES
        LINKS LINKS_PRIVATE
        INCLUDES INCLUDES_PRIVATE
        INCLUDES_BUILD INCLUDES_INSTALL
        DEFINES DEFINES_PRIVATE
        CCFLAGS CCFLAGS_PUBLIC
        QT_LINKS QT_LINKS_PRIVATE QT_INCLUDES_PRIVATE
    )
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    target_sources(${_target} PRIVATE ${FUNC_SOURCES})

    target_link_libraries(${_target} PUBLIC ${FUNC_LINKS})
    target_link_libraries(${_target} PRIVATE ${FUNC_LINKS_PRIVATE})

    target_include_directories(${_target} PUBLIC ${FUNC_INCLUDES})
    target_include_directories(${_target} PRIVATE ${FUNC_INCLUDES_PRIVATE})

    foreach(_item ${FUNC_INCLUDES_BUILD})
        target_include_directories(${_target} PUBLIC $<BUILD_INTERFACE:${_item}>)
    endforeach()

    foreach(_item ${FUNC_INCLUDES_INSTALL})
        target_include_directories(${_target} PUBLIC $<INSTALL_INTERFACE:${_item}>)
    endforeach()

    target_compile_definitions(${_target} PUBLIC ${FUNC_DEFINES})
    target_compile_definitions(${_target} PRIVATE ${FUNC_DEFINES_PRIVATE})

    target_compile_options(${_target} PUBLIC ${FUNC_CCFLAGS_PUBLIC})
    target_compile_options(${_target} PRIVATE ${FUNC_CCFLAGS})

    set(_qt_libs)
    ck_add_qt_module(_qt_libs ${FUNC_QT_LINKS})
    target_link_libraries(${_target} PUBLIC ${_qt_libs})

    set(_qt_libs_p)
    ck_add_qt_module(_qt_libs_p ${FUNC_QT_LINKS})
    target_link_libraries(${_target} PRIVATE ${_qt_libs_p})

    set(_qt_incs)
    ck_add_qt_private_inc(_qt_incs ${FUNC_QT_INCLUDES_PRIVATE})
    target_include_directories(${_target} PRIVATE ${_qt_incs})

    if(FUNC_INCLUDE_CURRENT)
        target_include_directories(${_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    elseif(FUNC_INCLUDE_CURRENT_PRIVATE)
        target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(FUNC_INCLUDE_SUBDIRS OR FUNC_INCLUDE_SUBDIRS_PRIVATE)
        ck_get_subdirs(_subdirs ABSOLUTE)

        if(FUNC_INCLUDE_SUBDIRS)
            target_include_directories(${_target} PUBLIC ${_subdirs})
        elseif(FUNC_INCLUDE_SUBDIRS_PRIVATE)
            target_include_directories(${_target} PRIVATE ${_subdirs})
        endif()
    endif()

    # ----------------- Template End -----------------
endfunction()

#[[
Attach embedded resource files to target on Windows, MacOSX.

    ck_target_res(<target>
        [NAME name]
        [VERSION version]
        [DESCRIPTION description]
        [COPYRIGHT copyright]
        [ICO ico]
        [ICNS icns]
        [MANIFEST]
    )

    Arguments:
        NAME: Default to `PROJECT_NAME`
        VERSION: Default to `PROJECT_VERSION`, should be the format: A.B.C[.D]
        DESCRIPTION: Default to `PROJECT_NAME`
        COPYRIGHT: Default to `PROJECT_NAME`
]] #
function(ck_target_res _target)
    set(options MANIFEST)
    set(oneValueArgs NAME VERSION DESCRIPTION COPYRIGHT ICO ICNS)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    if(FUNC_NAME)
        set(_app_name ${FUNC_NAME})
    else()
        set(_app_name ${PROJECT_NAME})
    endif()

    if(FUNC_VERSION)
        set(_app_version ${FUNC_VERSION})
    else()
        set(_app_version ${PROJECT_VERSION})
    endif()

    ck_parse_version(_app_version ${_app_version})

    if(FUNC_DESCRIPTION)
        set(_app_desc ${FUNC_DESCRIPTION})
    else()
        set(_app_desc ${PROJECT_NAME})
    endif()

    if(FUNC_COPYRIGHT)
        set(_app_copyright ${FUNC_COPYRIGHT})
    else()
        set(_app_copyright ${PROJECT_NAME})
    endif()

    # Correct output name if empty
    get_target_property(_org_output_name ${_target} OUTPUT_NAME)

    if(NOT _org_output_name AND NOT ${_target} STREQUAL ${_app_name})
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_app_name})
    endif()

    if(WIN32)
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
        configure_file(${CHORUSKIT_CMAKE_MODULES_DIR}/WinResource.rc.in ${_rc_path} @ONLY)
        target_sources(${_target} PRIVATE ${_rc_path})

        if(FUNC_MANIFEST)
            set(MANIFEST_VERSION ${_app_version})
            set(MANIFEST_IDENTIFIER ${_app_name})
            set(MANIFEST_DESCRIPTION ${_app_desc})

            set(_manifest_path ${CMAKE_CURRENT_BINARY_DIR}/${_target}.manifest)
            configure_file(${CHORUSKIT_CMAKE_MODULES_DIR}/WinManifest.manifest.in ${_manifest_path} @ONLY)
            target_sources(${_target} PRIVATE ${_manifest_path})
        endif()
    elseif(APPLE)
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
    endif()

    # ----------------- Template End -----------------
endfunction()

#[[
Add qt translation target.

    ck_add_translations(<target>
        LOCALES locales
        [SOURCES files... | TARGETS targets...]
        [PREFIX prefix]
        [TS_DIR dir]
        [QM_DIR dir]
        [TS_TARGET <target>]
        [QM_TARGET <target>]
    )

    Arguments:
        LOCALES: language names, e.g. zh_CN en_US
        SOURCES: source files
        TARGETS: target names, the source files of which will be collected
        PREFIX: translation file prefix, default to target name
        TS_DIR: ts files destination, default to `CMAKE_CURRENT_SOURCE_DIR`
        QM_DIR: qm files destination, default to `CMAKE_CURRENT_BINARY_DIR`
        TS_TARGET: update ts files before build target
        QM_TARGET: release qm files after build target
]] #
function(ck_add_translations _target)
    set(options)
    set(oneValueArgs PREFIX TS_DIR QM_DIR TS_TARGET QM_TARGET)
    set(multiValueArgs LOCALES SOURCES TARGETS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    if(FUNC_SOURCES)
        set(_src_files ${FUNC_SOURCES})
    elseif(FUNC_TARGETS)
        set(_src_files)

        foreach(_item ${FUNC_TARGETS})
            set(_tmp_files)
            get_target_property(_tmp_files ${_item} SOURCES)
            list(FILTER _tmp_files INCLUDE REGEX ".+\\.(cpp|cc)")
            list(FILTER _tmp_files EXCLUDE REGEX "(qasc|moc)_.+")
            list(APPEND _src_files ${_tmp_files})
            unset(_tmp_files)
        endforeach()
    else()
        message(FATAL_ERROR "ck_add_translations: either SOURCES or TARGETS should be specified!")
    endif()

    ck_find_qt_module(LinguistTools)

    if(FUNC_PREFIX)
        set(_prefix ${FUNC_PREFIX})
    else()
        set(_prefix ${_target})
    endif()

    if(FUNC_TS_DIR)
        set(_ts_dir ${FUNC_TS_DIR})
    else()
        set(_ts_dir ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(FUNC_QM_DIR)
        set(_qm_dir ${FUNC_QM_DIR})
    else()
        set(_qm_dir ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    set(_ts_files)

    foreach(_loc ${FUNC_LOCALES})
        list(APPEND _ts_files ${_ts_dir}/${_prefix}_${_loc}.ts)
    endforeach()

    set(_ts_target)
    if (FUNC_TS_TARGET)
        list(APPEND _ts_target PRE_BUILD_TARGET)
        list(APPEND _ts_target ${FUNC_TS_TARGET})
    endif()

    set(_qm_target)
    if (FUNC_QM_TARGET)
        list(APPEND _qm_target POST_BUILD_TARGET)
        list(APPEND _qm_target ${FUNC_QM_TARGET})
    endif()

    _ck_add_lupdate_target(${_target}_lupdate
        INPUT ${_src_files}
        OUTPUT ${_ts_files}
        CREATE_ONCE
        ${_ts_target}
    )

    _ck_add_lrelease_target(${_target}_lrelease
        INPUT ${_ts_files}
        DESTINATION ${_qm_dir}
        DEPENDS ${_target}_lupdate
        ${_qm_target}
    )

    add_custom_target(${_target})
    add_dependencies(${_target} ${_target}_lupdate)
    add_dependencies(${_target} ${_target}_lrelease)

    add_dependencies(ChorusKit_UpdateTranslations ${_target}_lupdate)
    add_dependencies(ChorusKit_ReleaseTranslations ${_target}_lrelease)

    # ----------------- Template End -----------------
endfunction()

#[[
Add a resources copying command after building the target.

    ck_add_attaches(<target>
        SRC <files1...> DEST <dir1>
        SRC <files2...> DEST <dir2> ...
    )
]] #
function(ck_add_attaches _target)
    set(_src)
    set(_dest)
    set(_status NONE) # NONE, SRC, DEST

    foreach(_item ${ARGN})
        if(${_item} STREQUAL SRC)
            if(${_status} STREQUAL NONE)
                set(_src)
                set(_status SRC)
            elseif(${_status} STREQUAL DEST)
                message(FATAL_ERROR "ck_add_attaches: missing directory name after DEST!")
            else()
                message(FATAL_ERROR "ck_add_attaches: missing source files after SRC!")
            endif()
        elseif(${_item} STREQUAL DEST)
            if(${_status} STREQUAL SRC)
                set(_status DEST)
            elseif(${_status} STREQUAL DEST)
                message(FATAL_ERROR "ck_add_attaches: missing directory name after DEST!")
            else()
                message(FATAL_ERROR "ck_add_attaches: no source files specified for DEST!")
            endif()
        else()
            if(${_status} STREQUAL NONE)
                message(FATAL_ERROR "ck_add_attaches: missing SRC or DEST token!")
            elseif(${_status} STREQUAL DEST)
                if(NOT _src)
                    message(FATAL_ERROR "ck_add_attaches: no source files specified for DEST!")
                endif()

                set(_status NONE)

                if(${_item} MATCHES ".*\$.*")
                    set(_path ${_item})
                else()
                    get_filename_component(_path ${_item} ABSOLUTE)
                endif()

                foreach(_src_item ${_src})
                    if(IS_DIRECTORY ${_src_item})
                        get_filename_component(_name ${_src_item} NAME)
                        add_custom_command(TARGET ${_target} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E make_directory ${_path}/${_name}
                            COMMAND ${CMAKE_COMMAND} -E copy_directory ${_src_item} ${_path}/${_name}
                        )
                    else()
                        file(GLOB _src_files ${_src_item})
                        add_custom_command(TARGET ${_target} POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E make_directory ${_path}
                            COMMAND ${CMAKE_COMMAND} -E copy ${_src_files} ${_path}
                        )
                    endif()
                endforeach()
            else()
                get_filename_component(_path ${_item} ABSOLUTE)
                list(APPEND _src ${_path})
            endif()
        endif()
    endforeach()

    if(${_status} STREQUAL SRC)
        message(FATAL_ERROR "ck_add_attaches: missing DEST after source files!")
    elseif(${_status} STREQUAL DEST)
        message(FATAL_ERROR "ck_add_attaches: missing directory name after DEST!")
    endif()
endfunction()

#[[
Add an application loader, add env vars and call `ENTRY_NAME`.

    ck_add_appmain(<target>
        [ENV envs...]
        [ENTRY_NAME name]
        [CONSOLE] [WINDOWS]
    )

    Arguments:
        ENTRY_NAME: Default to "main_entry"
]] #
function(ck_add_appmain _target _dll)
    set(options CONSOLE WINDOWS)
    set(oneValueArgs ENTRY_NAME)
    set(multiValueArgs ENV)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    set(_env_codes)

    if(FUNC_ENV)
        foreach(_item ${FUNC_ENV})
            set(_env_codes "${_env_codes}    PUTENV(\"${_item}\")\;\n")
        endforeach()

        set(APP_MAIN_ENVS ${_env_codes})
    endif()

    if(FUNC_ENTRY_NAME)
        set(_entry_name ${FUNC_ENTRY_NAME})
    else()
        set(_entry_name main_entry)
    endif()

    set(APP_MAIN_ENTRY_NAME ${_entry_name})

    set(_cpp_path ${CMAKE_CURRENT_BINARY_DIR}/${_target}_main.cpp)
    configure_file(
        ${CHORUSKIT_CMAKE_MODULES_DIR}/ChorusKitAppMain.cpp.in
        ${_cpp_path}
        @ONLY
    )
    add_executable(${_target} ${_cpp_path})
    target_link_libraries(${_target} PRIVATE ${_dll})

    if(FUNC_WINDOWS)
        set_target_properties(${_target} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    elseif(FUNC_CONSOLE)
        set_target_properties(${_target} PROPERTIES
            WIN32_EXECUTABLE FALSE
        )
    endif()

    # ----------------- Template End -----------------
endfunction()

#[[
Add a library, default to static library.

    ck_add_library(<target>
        [SHARED] [AUTOGEN]
        [MACRO_PREFIX   prefix]
        [TYPE_MACRO     macro]
        [LIBRARY_MACRO  macro]
    )

    Arguments:
        SHARED: Build shared library
        AUTOGEN: Enable CMAKE_AUTOUIC, CMAKE_AUTOMOC, CMAKE_AUTORCC
        MACRO_PREFIX: Default to upcase of `target`
        TYPE_MACRO: Default to `MACRO_PREFIX`_STATIC or `MACRO_PREFIX`_SHARED
        LIBRARY_MACRO: Default to `MACRO_PREFIX`_LIBRARY
]] #
function(ck_add_library _target)
    set(options SHARED AUTOGEN)
    set(oneValueArgs MACRO_PREFIX LIBRARY_MACRO TYPE_MACRO)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    if(FUNC_AUTOGEN)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
    endif()

    if(FUNC_MACRO_PREFIX)
        set(_prefix ${FUNC_MACRO_PREFIX})
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

    target_compile_definitions(${_target} PUBLIC ${_type_macro})
    target_compile_definitions(${_target} PRIVATE ${_library_macro})

    # ----------------- Template End -----------------
endfunction()

#[[
Create an executable shortcut after build.

    ck_add_win_shortcut(<target> <dir>
        [OUTPUT_NAME <name>]
    )
    
    Arguments:
        OUTPUT_NAME: output lnk name
]] #
function(ck_add_win_shortcut _target _dir)
    if(NOT WIN32)
        return()
    endif()

    set(options)
    set(oneValueArgs OUTPUT_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (FUNC_OUTPUT_NAME)
        set(_output_name ${FUNC_OUTPUT_NAME})
    else()
        set(_output_name $<TARGET_FILE_BASE_NAME:${_target}>)
    endif()

    string(RANDOM LENGTH 8 _rand)
    set(_vbs_name ${CMAKE_CURRENT_BINARY_DIR}/${_target}_shortcut.vbs)
    set(_vbs_temp ${_vbs_name}.in)

    set(SHORTCUT_PATH "${_dir}/${_output_name}.lnk")
    set(SHORTCUT_TARGET_PATH $<TARGET_FILE:${_target}>)
    set(SHORTCUT_WORKING_DIRECOTRY $<TARGET_FILE_DIR:${_target}>)
    set(SHORTCUT_DESCRIPTION $<TARGET_FILE_BASE_NAME:${_target}>)
    set(SHORTCUT_ICON_LOCATION $<TARGET_FILE:${_target}>)

    configure_file(
        ${CHORUSKIT_CMAKE_MODULES_DIR}/WinCreateShortcut.vbs.in
        ${_vbs_temp}
        @ONLY
    )
    file(GENERATE OUTPUT ${_vbs_name} INPUT ${_vbs_temp})

    add_custom_command(
        TARGET ${_target} POST_BUILD
        COMMAND cscript ${_vbs_name}
    )
endfunction()

#[[
Deploy Qt libraries and plugins.

    ck_add_deploy_qt_target(<target>
        [LIB_DIR <dir>]
        [PLUGINS_DIR <dir>]
    )

    Should specify a target.
]] #
function(ck_add_deploy_qt_target _target)
    set(options)
    set(oneValueArgs LIB_DIR PLUGINS_DIR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
    # string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
    # string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)

    # set(_release_name ${PROJECT_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})
    # set(_deploy_dir ${CMAKE_BINARY_DIR}/${_release_name})

    # Find Qt tools
    if(NOT DEFINED QT_QMAKE_EXECUTABLE)
        get_target_property(QT_QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    endif()

    if(NOT EXISTS "${QT_QMAKE_EXECUTABLE}")
        message(WARNING "Cannot find the QMake executable.")
        return()
    endif()

    get_filename_component(QT_BIN_DIRECTORY "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

    find_program(QT_DEPLOY_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${QT_BIN_DIRECTORY}")

    if(NOT EXISTS "${QT_DEPLOY_EXECUTABLE}")
        message(WARNING "Cannot find the deployqt tool.")
        return()
    endif()

    if(FUNC_LIB_DIR)
        set(_lib_dir ${FUNC_LIB_DIR})
    else()
        set(_lib_dir $<TARGET_FILE_DIR:${_target}>)
    endif()

    if(FUNC_PLUGINS_DIR)
        set(_plugins_dir ${FUNC_PLUGINS_DIR})
    else()
        set(_plugins_dir $<TARGET_FILE_DIR:${_target}>/plugins)
    endif()

    add_custom_target(ChorusKit_DeployQt
        COMMAND ${QT_DEPLOY_EXECUTABLE}
        --libdir ${_lib_dir}
        --plugindir ${_plugins_dir}
        --no-translations
        --no-system-d3d-compiler

        # --no-virtualkeyboard
        --no-compiler-runtime
        --no-opengl-sw
        --force
        --verbose 0
        $<TARGET_FILE:${_target}>
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${_target}>
    )
endfunction()
