include_guard(GLOBAL)

# ----------------------------------
# Project information
# ----------------------------------
set(CHORUSKIT_VERSION 0.0.1.9)

set(CK_QT_VERSION_MIN 5.15.2)
set(CK_DEV_START_YEAR 2019)

# ----------------------------------
# ChorusKit API
# ----------------------------------
include(${CMAKE_CURRENT_LIST_DIR}/ChorusKitAPIInternal.cmake)

#[[
Initialize ChorusKitApi global settings.

    ck_init_buildsystem()
]] #
function(ck_init_buildsystem)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # if(APPLE AND CK_ENABLE_DEVEL)
    # message(FATAL_ERROR "ck_init_buildsystem: Develop mode is not support on Mac.")
    # endif()
    if(NOT CHORUSKIT_REPOSITORY AND CK_ENABLE_DEVEL)
        message(FATAL_ERROR "ck_init_buildsystem: Develop mode is not support outside ChorusKit repository.")
    endif()

    # Store data during configuration
    add_custom_target(ChorusKit_Metadata)

    # Update all translations
    add_custom_target(ChorusKit_UpdateTranslations)

    # Release all translations
    add_custom_target(ChorusKit_ReleaseTranslations)

    # Copy global shared files
    add_custom_target(ChorusKit_CopySharedFiles)
endfunction()

#[[
Add target to copy vcpkg dependencies on Windows.

    ck_init_vcpkg(<vcpkg_dir> <vcpkg_triplet>)
]] #
function(ck_init_vcpkg _vcpkg_dir _vcpkg_triplet)
    if(NOT WIN32 OR NOT CK_ENABLE_VCPKG_DEPS)
        return()
    endif()

    # Deploy vcpkg dependencies
    set(_vcpkg_installed_dir ${_vcpkg_dir}/installed)
    set(_vcpkg_triplet_suffix)

    if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
        set(_vcpkg_triplet_suffix "/debug")
    endif()

    if(WIN32)
        set(_bin_dir ${_vcpkg_installed_dir}/${_vcpkg_triplet}${_vcpkg_triplet_suffix}/bin)
        set(_runtime_output_dir ${CK_GLOBAL_RUNTIME_OUTPUT_PATH})
        set(_install_dir bin)
    else()
        set(_bin_dir ${_vcpkg_installed_dir}/${_vcpkg_triplet}${_vcpkg_triplet_suffix}/lib)
        set(_runtime_output_dir ${CK_GLOBAL_LIBRARY_OUTPUT_PATH})
        set(_install_dir lib)
    endif()

    set(_prebuilt_dlls)
    file(GLOB _dlls ${_bin_dir}/${CK_SHARED_LIBRARY_PATTERN})

    foreach(_dll ${_dlls})
        get_filename_component(_name ${_dll} NAME)
        set(_out_dll ${_runtime_output_dir}/${_name})

        add_custom_command(
            OUTPUT ${_out_dll}
            DEPENDS ${_dll}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_runtime_output_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_dll} ${_runtime_output_dir}
        )

        list(APPEND _prebuilt_dlls ${_out_dll})

        install(FILES ${_dll} DESTINATION ${_install_dir})
    endforeach()

    add_custom_target(ChorusKit_SetupVcpkgDeps ALL DEPENDS ${_prebuilt_dlls})
endfunction()

#[[
Add compile definitions to ChorusKit config header.

    ck_add_definition(<key> [value])
]] #
function(ck_add_definition)
    set(_def)

    set(_list ${ARGN})
    list(LENGTH _list _len)

    if(${_len} EQUAL 1)
        set(_def ${_list})
    elseif(${_len} EQUAL 2)
        # Get key
        list(POP_FRONT _list _key)
        list(POP_FRONT _list _val)

        # Boolean
        if(${_val} STREQUAL "off")
            return()
        elseif(${_val} STREQUAL "on")
            set(_def ${_key})
        else()
            set(_def "${_key}=${_val}")
        endif()
    else()
        message(FATAL_ERROR "ck_add_definition: called with incorrect number of arguments")
    endif()

    # Add definition to global list
    _ck_property_list_append(ChorusKit_Metadata CONFIG_DEFINITIONS "${_def}")
endfunction()

#[[
Add test target, won't be installed.

    ck_add_test(<target> [sources]
        [CONSOLE] [WINDOWS] [NO_SKIP_INSTALL] [SKIP_EXPORT]
    )
]] #
function(ck_add_test _target)
    set(options CONSOLE WINDOWS NO_SKIP_INSTALL SKIP_EXPORT)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${_target} ${FUNC_UNPARSED_ARGUMENTS})

    if(WIN32 AND NOT FUNC_CONSOLE)
        if(FUNC_WINDOWS)
            set_target_properties(${_target} PROPERTIES WIN32_EXECUTABLE TRUE)
        else()
            # Set windows application type
            _ck_set_win32_executable_type(${_target})
        endif()
    endif()

    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CK_GLOBAL_RUNTIME_OUTPUT_PATH}
    )

    if(FUNC_NO_SKIP_INSTALL)
        if(FUNC_SKIP_EXPORT OR NOT CK_ENABLE_DEVEL)
            set(_export)
        else()
            set(_export EXPORT ${CK_INSTALL_EXPORT})
        endif()

        install(TARGETS ${_target}
            ${_export}
            RUNTIME DESTINATION ${CK_INSTALL_RUNTIME_OUTPUT_PATH} OPTIONAL
        )
    endif()
endfunction()

#[[
Add a library, default to static library.

    ck_add_library(<target>
        [SHARED] [AUTOGEN] [SKIP_INSTALL] [SKIP_EXPORT]
        [NAME           name] 
        [VERSION        version] 
        [DESCRIPTION    desc]
        [COPYRIGHT copyright | VENDOR vendor]
        [MACRO_PREFIX   prefix]
        [TYPE_MACRO     macro]
        [LIBRARY_MACRO  macro]
    )
]] #
function(ck_add_library _target)
    set(options AUTOGEN SKIP_INSTALL SKIP_EXPORT)
    set(oneValueArgs COPYRIGHT VENDOR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Check target name (Try)
    set(_ns)
    set(_name)
    _ck_parse_namespace(${_target} _ns _name)

    if(NOT _ns OR NOT _name)
        set(_ns)
        set(_name)
    endif()

    # Add Qt Moc
    if(FUNC_AUTOGEN)
        _ck_set_cmake_autoxxx(on)
    endif()

    # Add library target and attach definitions
    _ck_add_library_internal(${_target} ${FUNC_UNPARSED_ARGUMENTS})

    if(_ns)
        add_library(${_ns}::${_name} ALIAS ${_target})

        # Add target level dependency
        add_dependencies(${_ns} ${_target})

        # Set parsed name as output name if not set
        _ck_try_set_output_name(${_target} ${_name})
    endif()

    # Get target type
    _ck_check_shared_library(${_target} _shared)

    # Add windows rc file
    if(WIN32)
        string(TIMESTAMP _year "%Y")

        if(FUNC_COPYRIGHT)
            set(_copyright ${FUNC_COPYRIGHT})
        else()
            _ck_set_value(_vendor FUNC_VENDOR "OpenVPI")
            set(_copyright "Copyright ${CK_DEV_START_YEAR}-${_year} ${_vendor}")
        endif()

        get_target_property(_type ${_target} TYPE)

        if(_shared)
            _ck_attach_win_rc_file(${_target}
                COPYRIGHT "${_copyright}"
                ${FUNC_UNPARSED_ARGUMENTS}
            )
        endif()
    endif()

    if(_ns)
        if(APPLE)
            set(_build_output_dir $<TARGET_BUNDLE_DIR:${_ns}>/Contents/Frameworks)
            set(_install_output_dir $<TARGET_FILE_NAME:${_ns}>.app/Contents/Frameworks)
        else()
            set(_build_output_dir ${CK_GLOBAL_LIBRARY_OUTPUT_PATH}/${_ns})
            set(_install_output_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/${_ns})
        endif()
    else()
        set(_build_output_dir ${CK_GLOBAL_LIBRARY_OUTPUT_PATH}/ChorusKit)
        set(_install_output_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/ChorusKit)
    endif()

    # Set output directories
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CK_GLOBAL_RUNTIME_OUTPUT_PATH}
        LIBRARY_OUTPUT_DIRECTORY ${_build_output_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_build_output_dir}
    )

    # Install target
    if(FUNC_SKIP_EXPORT OR NOT CK_ENABLE_DEVEL)
        set(_export)
    else()
        set(_export EXPORT ${CK_INSTALL_EXPORT})
    endif()

    if(NOT FUNC_SKIP_INSTALL)
        if(CK_ENABLE_DEVEL)
            install(TARGETS ${_target}
                ${_export}
                RUNTIME DESTINATION ${CK_INSTALL_RUNTIME_OUTPUT_PATH}
                LIBRARY DESTINATION ${_install_output_dir}
                ARCHIVE DESTINATION ${_install_output_dir}
            )
        elseif(_shared)
            install(TARGETS ${_target}
                ${_export}
                RUNTIME DESTINATION ${CK_INSTALL_RUNTIME_OUTPUT_PATH}
                LIBRARY DESTINATION ${_install_output_dir}
            )
        endif()
    endif()

    if(_ns)
        # Add target to global list
        _ck_property_list_append(${_ns} LIBRARIES ${_target})
    else()
        # Add target to global list
        _ck_property_list_append(ChorusKit_Metadata LIBRARY_TARGETS ${_target})
    endif()
endfunction()

#[[
Add a library plugin, name must be in the form of XXX__XXX.

    ck_add_library_plugin(<target>
        [SKIP_EXPORT]
        [NAME           name] 
        [VERSION        version] 
        [DESCRIPTION    desc]
        [COPYRIGHT copyright | VENDOR vendor]
        [MACRO_PREFIX   prefix]
        [TYPE_MACRO     macro]
        [LIBRARY_MACRO  macro]
    )
]] #
function(ck_add_library_plugin _target _category)
    set(options SKIP_EXPORT)
    set(oneValueArgs COPYRIGHT VENDOR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Check target name
    set(_ns)
    set(_name)
    _ck_parse_namespace(${_target} _ns _name)

    if(NOT _ns OR NOT _name)
        message(FATAL_ERROR "ck_add_library_plugin: target name must be in the form of \"XXX__XXX\"")
    endif()

    # Add Qt Moc
    _ck_set_cmake_autoxxx(on)

    # Add library target and attach definitions
    _ck_add_library_internal(${_target} SHARED ${FUNC_UNPARSED_ARGUMENTS})
    add_library(${_ns}::${_name} ALIAS ${_target})

    # Set parsed name as output name if not set
    _ck_try_set_output_name(${_target} ${_name})

    # Add windows rc file
    if(WIN32)
        string(TIMESTAMP _year "%Y")

        if(FUNC_COPYRIGHT)
            set(_copyright ${FUNC_COPYRIGHT})
        else()
            _ck_set_value(_vendor FUNC_VENDOR "OpenVPI")
            set(_copyright "Copyright ${CK_DEV_START_YEAR}-${_year} ${_vendor}")
        endif()

        _ck_attach_win_rc_file(${_target}
            COPYRIGHT "${_copyright}"
            ${FUNC_UNPARSED_ARGUMENTS}
        )
    endif()

    set(_output_dir ChorusKit/plugins/${_category})
    set(_build_output_dir ${CK_GLOBAL_LIBRARY_OUTPUT_PATH}/${_output_dir})
    set(_install_output_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/${_output_dir})

    # Set output directories
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_build_output_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_build_output_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_build_output_dir}
    )

    # Install target
    if(FUNC_SKIP_EXPORT OR NOT CK_ENABLE_DEVEL)
        set(_export)
    else()
        set(_export EXPORT ${CK_INSTALL_EXPORT})
    endif()

    if(CK_ENABLE_DEVEL)
        install(TARGETS ${_target}
            ${_export}
            RUNTIME DESTINATION ${_install_output_dir}
            LIBRARY DESTINATION ${_install_output_dir}
            ARCHIVE DESTINATION ${_install_output_dir}
        )
    else()
        install(TARGETS ${_target}
            ${_export}
            RUNTIME DESTINATION ${_install_output_dir}
            LIBRARY DESTINATION ${_install_output_dir}
        )
    endif()

    # Add target to global list
    _ck_property_list_append(ChorusKit_Metadata LIBRARY_PLUGIN_TARGETS ${_target})
endfunction()

#[[
Add an application, need to specify an entry library.

    ck_add_application(<target> <entry library> <ICO ico> <ICNS icns>
        [SKIP_EXPORT]
        [NAME           name] 
        [VERSION        version] 
        [DESCRIPTION    desc]
        [ENTRY_NAME     name]
    )
]] #
function(ck_add_application _target _entry_library)
    set(options SKIP_EXPORT)
    set(oneValueArgs ENTRY_NAME ICO ICNS)
    set(multiValueArgs ENV)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Configure main cpp template
    _ck_set_value(_entry_name FUNC_ENTRY_NAME "main_entry")
    set(APP_MAIN_ENTRY_NAME ${_entry_name})

    if(FUNC_ENV)
        foreach(_item ${FUNC_ENV})
            set(_env_codes "${_env_codes}    PUTENV(\"${_item}\")\;\n")
        endforeach()

        set(APP_MAIN_ENVS ${_env_codes})
    endif()

    set(_cpp_path ${CMAKE_CURRENT_BINARY_DIR}/${_target}_main.cpp)
    configure_file(
        ${CK_CMAKE_MODULES_DIR}/ChorusKitAppMain.cpp.in
        ${_cpp_path}
        @ONLY
    )

    # Add target
    add_executable(${_target} ${_cpp_path})
    target_link_libraries(${_target} PRIVATE ${_entry_library})

    # Make location dependent executable, otherwise GNOME cannot recognize
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        target_link_options(${_target} PRIVATE "-no-pie")
    endif()

    string(TIMESTAMP _year "%Y")
    set(_copyright "Copyright ${CK_DEV_START_YEAR}-${_year} OpenVPI")

    if(APPLE)
        # Add mac bundle
        _ck_attach_mac_bundle(${_target}
            COPYRIGHT "${_copyright}"
            ICNS ${FUNC_ICNS}
            ${FUNC_UNPARSED_ARGUMENTS}
        )
        set_target_properties(${_target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CK_MAIN_OUTPUT_PATH}
            OUTPUT_NAME "ChorusKit ${_target}"
        )
    else()
        if(WIN32)
            # Set windows application type
            _ck_set_win32_executable_type(${_target})

            # Add windows rc file
            _ck_attach_win_rc_file(${_target}
                MANIFEST
                COPYRIGHT "${_copyright}"
                ICO ${FUNC_ICO}
                ${FUNC_UNPARSED_ARGUMENTS}
            )
        endif()

        set_target_properties(${_target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CK_GLOBAL_RUNTIME_OUTPUT_PATH}
        )

        # Add shortcut
        _ck_create_win_shortcut(${_target} ${CK_MAIN_OUTPUT_PATH}
            OUTPUT_NAME "ChorusKit ${_target}"
        )
    endif()

    if(FUNC_SKIP_EXPORT OR NOT CK_ENABLE_DEVEL)
        set(_export)
    else()
        set(_export EXPORT ${CK_INSTALL_EXPORT})
    endif()

    if(APPLE)
        # Install to .
        install(TARGETS ${_target}
            ${_export}
            DESTINATION . OPTIONAL
        )
    else()
        # Install to bin
        install(TARGETS ${_target}
            ${_export}
            DESTINATION ${CK_INSTALL_RUNTIME_OUTPUT_PATH} OPTIONAL
        )
    endif()

    # Add post build events to distribute shared files
    _ck_dist_shared_for_application(${_target})

    # Add target to global list
    _ck_property_list_append(ChorusKit_Metadata APPLICATION_TARGETS ${_target})
endfunction()

#[[
Add an application plugin, name must be in the form of XXX__XXX.

    ck_add_application_plugin(<target>
        [SKIP_EXPORT]   [NO_PLUGIN_JSON]
        [CATEGORY       category]
        [NAME           name] 
        [VERSION        version] 
        [DESCRIPTION    desc]
        [VENDOR         vendor]
        [PLUGIN_JSON    plugin.json.in]
        [COMPAT_VERSION compat_version]
        [MACRO_PREFIX   prefix]
        [TYPE_MACRO     macro]
        [LIBRARY_MACRO  macro]
    )
]] #
function(ck_add_application_plugin _target)
    set(options SKIP_EXPORT)
    set(oneValueArgs VENDOR PLUGIN_JSON)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_ns)
    set(_name)
    _ck_parse_namespace(${_target} _ns _name)

    if(NOT _ns OR NOT _name)
        message(FATAL_ERROR "ck_add_library_plugin: target name must be in the form of \"XXX__XXX\"")
    endif()

    # Add Qt Moc
    _ck_set_cmake_autoxxx(on)

    # Add library target and attach definitions
    _ck_add_library_internal(${_target} SHARED ${FUNC_UNPARSED_ARGUMENTS})
    add_library(${_ns}::${_name} ALIAS ${_target})

    # Add target level dependency
    add_dependencies(${_ns} ${_target})

    # Set parsed name as output name if not set
    _ck_try_set_output_name(${_target} ${_name})

    _ck_set_value(_vendor FUNC_VENDOR "OpenVPI")

    if(WIN32)
        string(TIMESTAMP _year "%Y")

        set(_copyright "Copyright ${CK_DEV_START_YEAR}-${_year} ${_vendor}")

        # Add windows rc file
        _ck_attach_win_rc_file(${_target}
            COPYRIGHT "${_copyright}"
            ${FUNC_UNPARSED_ARGUMENTS}
        )
    endif()

    # Configure plugin json if specified
    if(FUNC_PLUGIN_JSON)
        ck_configure_plugin_metadata(${_target} ${FUNC_PLUGIN_JSON} ${FUNC_UNPARSED_ARGUMENTS} VENDOR ${_vendor})
    elseif(NOT FUNC_NO_PLUGIN_JSON AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/plugin.json.in)
        ck_configure_plugin_metadata(${_target} plugin.json.in ${FUNC_UNPARSED_ARGUMENTS} VENDOR ${_vendor})
    endif()

    # Configure plugin desc file
    set(_tmp_desc_file ${CMAKE_CURRENT_BINARY_DIR}/${_name}Metadata/plugin.json)
    _ck_configure_plugin_desc(${_tmp_desc_file} ${FUNC_UNPARSED_ARGUMENTS})
    ck_add_attached_files(${_target}
        SRC ${_tmp_desc_file} DEST .
    )

    _ck_set_value(_category FUNC_CATEGORY ${PROJECT_NAME})

    if(APPLE)
        if(CHORUSKIT_REPOSITORY)
            set(_build_output_dir ${CK_MAIN_OUTPUT_PATH}/${_ns}/plugins/${_category})
        else()
            set(_build_output_dir $<TARGET_BUNDLE_DIR:${_ns}>/Contents/Plugins/${_category})
        endif()

        set(_install_output_dir $<TARGET_FILE_NAME:${_ns}>.app/Contents/Plugins/${_category})
    else()
        set(_build_output_dir ${CK_GLOBAL_LIBRARY_OUTPUT_PATH}/${_ns}/plugins/${_category})
        set(_install_output_dir ${CK_INSTALL_LIBRARY_OUTPUT_PATH}/${_ns}/plugins/${_category})
    endif()

    # Set output directories
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_build_output_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_build_output_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_build_output_dir}
    )

    # Install target
    if(FUNC_SKIP_EXPORT)
        set(_export)
    else()
        set(_export EXPORT ${CK_INSTALL_EXPORT})
    endif()

    if(CK_ENABLE_DEVEL)
        install(TARGETS ${_target}
            ${_export}
            RUNTIME DESTINATION ${_install_output_dir}
            LIBRARY DESTINATION ${_install_output_dir}
            ARCHIVE DESTINATION ${_install_output_dir}
        )
    else()
        install(TARGETS ${_target}
            ${_export}
            RUNTIME DESTINATION ${_install_output_dir}
            LIBRARY DESTINATION ${_install_output_dir}
        )
    endif()

    # Add plugin to target's plugin list
    _ck_property_list_append(${_ns} PLUGINS ${_name})
endfunction()

#[[
Add an application plugin, name must be in the form of XXX::XXX.

    ck_configure_plugin_metadata(<target>
        [NAME               name            ] 
        [VERSION            version         ] 
        [COMPAT_VERSION     compat_version  ]
        [VENDOR             vendor          ]
    )
]] #
function(ck_configure_plugin_metadata _target _plugin_json)
    set(options)
    set(oneValueArgs NAME VERSION COMPAT_VERSION VENDOR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Set plugin metadata
    _ck_set_value(_name FUNC_NAME ${PROJECT_NAME})
    _ck_set_value(_version FUNC_VERSION ${PROJECT_VERSION})
    _ck_set_value(_compat_version FUNC_COMPAT_VERSION "0.0.0.0")
    _ck_set_value(_vendor FUNC_VENDOR "ChorusKit")

    # Fix version
    ck_parse_version(_ver ${_version})
    set(PLUGIN_METADATA_VERSION ${_ver_1}.${_ver_2}.${_ver_3}_${_ver_4})

    ck_parse_version(_compat ${_compat_version})
    set(PLUGIN_METADATA_COMPAT_VERSION ${_compat_1}.${_compat_2}.${_compat_3}_${_compat_4})
    set(PLUGIN_METADATA_VENDOR ${_vendor})

    # Get year
    string(TIMESTAMP _year "%Y")
    set(PLUGIN_METADATA_YEAR "${_year}")

    configure_file(
        ${_plugin_json}
        ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata/plugin.json
        @ONLY
    )
    target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata)
endfunction()

#[[
Add a resources copying command after building the target.

    ck_add_attached_files(<target>
        [SKIP_BUILD] [SKIP_INSTALL]
        SRC <files1...> DEST <dir1>
        SRC <files2...> DEST <dir2> ...
    )
]] #
function(ck_add_attached_files _target)
    set(options SKIP_BUILD SKIP_INSTALL)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_error)
    _ck_resolve_src_dest("${FUNC_UNPARSED_ARGUMENTS}" _result _error)

    if(_error)
        message(FATAL_ERROR "ck_add_attached_files: ${_error}")
    endif()

    set(_options)

    if(FUNC_SKIP_BUILD)
        list(APPEND _options SKIP_BUILD)
    elseif(FUNC_SKIP_INSTALL)
        list(APPEND _options SKIP_INSTALL)
    endif()

    foreach(_item ${_result})
        list(POP_BACK _item _dest)
        _ck_add_copy_command(${_target} "$<TARGET_FILE_DIR:${_target}>" "${_item}" ${_dest} ${_options})
    endforeach()
endfunction()

#[[
Add a resources copying command after building the application.

    ck_add_application_files(<target>
        [SKIP_BUILD] [SKIP_INSTALL] [BASE_SHARE_DIR]
        SRC <files1...> DEST <dir1>
        SRC <files2...> DEST <dir2> ...
    )
]] #
function(ck_add_application_files _target)
    set(options SKIP_BUILD SKIP_INSTALL BASE_SHARE_DIR)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_error)
    _ck_resolve_src_dest("${FUNC_UNPARSED_ARGUMENTS}" _result _error)

    if(_error)
        message(FATAL_ERROR "ck_add_application_files: ${_error}")
    endif()

    set(_options)

    if(FUNC_SKIP_BUILD)
        list(APPEND _options SKIP_BUILD)
    elseif(FUNC_SKIP_INSTALL)
        list(APPEND _options SKIP_INSTALL)
    endif()

    if(APPLE)
        set(_share_dir "$<TARGET_BUNDLE_DIR:${_target}>/Contents/Resources")
    else()
        if(FUNC_BASE_SHARE_DIR)
            set(_share_dir ${CK_GLOBAL_SHARE_OUTPUT_PATH})
        else()
            set(_share_dir ${CK_GLOBAL_SHARE_OUTPUT_PATH}/${_target})
        endif()
    endif()

    foreach(_item ${_result})
        list(POP_BACK _item _dest)
        _ck_add_copy_command(${_target} ${_share_dir} "${_item}" ${_dest} ${_options})
    endforeach()
endfunction()

#[[
Add a resources copying command for whole project.

    ck_add_shared_files(
        SRC <files1...> DEST <dir1>
        SRC <files2...> DEST <dir2> ...
    )
]] #
function(ck_add_shared_files)
    set(options SKIP_BUILD SKIP_INSTALL)
    set(oneValueArgs)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_error)
    _ck_resolve_src_dest("${FUNC_UNPARSED_ARGUMENTS}" _result _error)

    if(_error)
        message(FATAL_ERROR "ck_add_shared_files: ${_error}")
    endif()

    set(_options)

    if(FUNC_SKIP_BUILD)
        list(APPEND _options SKIP_BUILD)
    elseif(FUNC_SKIP_INSTALL)
        list(APPEND _options SKIP_INSTALL)
    endif()

    foreach(_item ${_result})
        list(POP_BACK _item _dest)

        string(RANDOM LENGTH ${CK_CMAKE_RANDOM_LENGTH} _rand)
        set(_attach_target shared_copy_command_${_rand})
        add_custom_target(${_attach_target})
        add_dependencies(ChorusKit_CopySharedFiles ${_attach_target})

        _ck_add_copy_command(${_attach_target} ${CK_GLOBAL_SHARE_OUTPUT_PATH} "${_item}" ${_dest} ${_options})
    endforeach()
endfunction()

#[[
Finish ChorusKitApi global settings.

    ck_finish_build_system()
]] #
function(ck_finish_build_system)
    # Generate config header
    get_target_property(_def_list ChorusKit_Metadata CONFIG_DEFINITIONS)
    _ck_generate_config_header("${_def_list}" "${CK_HEADERS_OUTPUT_PATH}/choruskit_config.h")

    # Generate build info header
    _ck_configure_build_info_header("${CK_HEADERS_OUTPUT_PATH}/choruskit_buildinfo.h")

    if(NOT CHORUSKIT_REPOSITORY)
        return()
    endif()

    get_target_property(_app_list ChorusKit_Metadata APPLICATION_TARGETS)
    get_target_property(_lib_list ChorusKit_Metadata LIBRARY_TARGETS)
    get_target_property(_lib_plugin_list ChorusKit_Metadata LIBRARY_PLUGIN_TARGETS)

    # Add Deploy
    if(CK_ENABLE_DEPLOY_QT)
        _ck_deploy_qt_library()
    endif()

    if(APPLE)
        # Add install command to copy shared files
        foreach(_item ${_app_list})
            install(
                DIRECTORY ${CK_GLOBAL_LIBRARY_OUTPUT_PATH} ${CK_GLOBAL_SHARE_OUTPUT_PATH}
                DESTINATION $<TARGET_FILE_NAME:${_item}>.app/Contents
            )
        endforeach()
    endif()

    # Fix rpath and post deploy
    if(Python_FOUND)
        set(_script "${CK_PYTHON_SCRIPTS_DIR}/postdeploy.py")
        install(CODE "
            message(STATUS \"Post deploy: run ${_script}\")
            execute_process(
                COMMAND \"${Python_EXECUTABLE}\" \"${_script}\" \"${CMAKE_INSTALL_PREFIX}\"
                WORKING_DIRECTORY \"${CMAKE_INSTALL_PREFIX}\"
            )")
    endif()

    if(CK_ENABLE_DEVEL)
        include(GNUInstallDirs)
        include(CMakePackageConfigHelpers)

        # Add version file
        write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/ChorusKitConfigVersion.cmake"
            VERSION ${CHORUSKIT_VERSION}
            COMPATIBILITY AnyNewerVersion
        )

        # Add configuration file
        configure_package_config_file(
            "${CK_CMAKE_MODULES_DIR}/ChorusKitConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/ChorusKitConfig.cmake"
            INSTALL_DESTINATION ${CK_INSTALL_CMAKE_DIR}
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
        )

        # Install cmake files
        install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/ChorusKitConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/ChorusKitConfigVersion.cmake"
            DESTINATION ${CK_INSTALL_CMAKE_DIR}
        )

        # Install cmake targets files
        install(EXPORT ${CK_INSTALL_EXPORT}
            DESTINATION ${CK_INSTALL_CMAKE_DIR}
        )

        install(DIRECTORY "${CK_CMAKE_MODULES_DIR}/"
            DESTINATION ${CK_INSTALL_CMAKE_DIR}
        )
    endif()
endfunction()

#[[
CMake target commands wrapper to add sources, links, includes.

    ck_target_components(<target>
        [SOURCES files...]

        [LINKS            libs...]
        [LINKS_PRIVATE    libs...]
        
        [DEFINES          defs...]
        [DEFINES_PRIVATE  defs...]

        [CCFLAGS          flags...]
        [CCFLAGS_PRIVATE  flags...]

        [QT_LINKS             modules...]
        [QT_LINKS_PRIVATE     modules...]
        [QT_INCLUDES_PRIVATE  modules...]

        [AUTO_INCLUDE_CURRENT]
        [AUTO_INCLUDE_SUBDIRS]
        [AUTO_INCLUDE_DIRS dirs...]

        [INCLUDE_CURRENT_PRIVATE]
        [INCLUDE_SUBDIRS_PRIVATE]
        [INCLUDE_PRIVATE dirs...]
    )
]] #
function(ck_target_components _target)
    set(options AUTO_INCLUDE_CURRENT INCLUDE_CURRENT_PRIVATE AUTO_INCLUDE_SUBDIRS INCLUDE_SUBDIRS_PRIVATE)
    set(oneValueArgs)
    set(multiValueArgs SOURCES
        LINKS LINKS_PRIVATE
        DEFINES DEFINES_PRIVATE
        CCFLAGS CCFLAGS_PUBLIC
        QT_LINKS QT_LINKS_PRIVATE QT_INCLUDES_PRIVATE
        AUTO_INCLUDE_DIRS INCLUDE_PRIVATE
    )
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    target_sources(${_target} PRIVATE ${FUNC_SOURCES})
    target_include_directories(${_target} PRIVATE ${FUNC_INCLUDE_PRIVATE})

    target_link_libraries(${_target} PUBLIC ${FUNC_LINKS})
    target_link_libraries(${_target} PRIVATE ${FUNC_LINKS_PRIVATE})

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

    if(FUNC_INCLUDE_CURRENT_PRIVATE)
        target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    elseif(FUNC_AUTO_INCLUDE_CURRENT)
        file(RELATIVE_PATH _rel_path ${CK_CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
        target_include_directories(${_target}
            PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/.."
            "$<INSTALL_INTERFACE:${CK_INSTALL_INCLUDE_DIR}/${_rel_path}>"
            "$<INSTALL_INTERFACE:${CK_INSTALL_INCLUDE_DIR}/${_rel_path}>/.."
        )
    endif()

    if(FUNC_AUTO_INCLUDE_SUBDIRS OR FUNC_INCLUDE_SUBDIRS_PRIVATE)
        ck_get_subdirs(_subdirs ABSOLUTE)

        if(FUNC_INCLUDE_SUBDIRS_PRIVATE)
            target_include_directories(${_target} PRIVATE ${_subdirs})
        else()
            foreach(_abs_dir ${_subdirs})
                file(RELATIVE_PATH _rel_path ${CK_CMAKE_SOURCE_DIR} ${_abs_dir})
                target_include_directories(${_target}
                    PUBLIC
                    "$<BUILD_INTERFACE:${_abs_dir}>"
                    "$<INSTALL_INTERFACE:${CHORUSKIT_RELATIVE_INCLUDE_DIR}/${_rel_path}>"
                )
            endforeach()
        endif()
    endif()

    if(FUNC_AUTO_INCLUDE_DIRS)
        foreach(_item ${FUNC_AUTO_INCLUDE_DIRS})
            get_filename_component(_abs_dir ${_item} ABSOLUTE)
            file(RELATIVE_PATH _rel_path ${CK_CMAKE_SOURCE_DIR} ${_abs_dir})
            target_include_directories(${_target}
                PUBLIC
                "$<BUILD_INTERFACE:${_abs_dir}>"
                "$<INSTALL_INTERFACE:${CHORUSKIT_RELATIVE_INCLUDE_DIR}/${_rel_path}>"
            )
        endforeach()
    endif()

    # ----------------- Template End -----------------
endfunction()

#[[
Add qt translation target.

    ck_add_translations(<target>
        [LOCALES locales]
        [SOURCES files... | TARGETS targets... | TS_FILES files...]
        [PREFIX prefix]
        [TS_DIR dir]
        [QM_DIR dir]
        [TS_OPTIONS options...]
        [QM_OPTIONS options...]
    )

    Arguments:
        LOCALES: language names, e.g. zh_CN en_US, must specify if SOURCES or TARGETS is specified
        SOURCES: source files
        TARGETS: target names, the source files of which will be collected
        TS_FILES: ts file names, add the specified ts file
        PREFIX: translation file prefix, default to target name
        TS_DIR: ts files destination, default to `CMAKE_CURRENT_SOURCE_DIR`
        QM_DIR: qm files destination, default to `CMAKE_CURRENT_BINARY_DIR`
]] #
function(ck_add_translations _target)
    set(options)
    set(oneValueArgs PREFIX TS_DIR QM_DIR)
    set(multiValueArgs LOCALES SOURCES TARGETS TS_FILES TS_OPTIONS QM_OPTIONS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    set(_src_files)
    set(_include_dirs)

    # Collect source files
    if(FUNC_SOURCES)
        list(APPEND _src_files ${FUNC_SOURCES})
    endif()

    # Collect source files
    if(FUNC_TARGETS)
        foreach(_item ${FUNC_TARGETS})
            get_target_property(_type ${_item} TYPE)

            if(${_type} STREQUAL "UTILITY")
                continue()
            endif()

            set(_tmp_files)
            get_target_property(_tmp_files ${_item} SOURCES)
            list(FILTER _tmp_files INCLUDE REGEX ".+\\.(cpp|cc)")
            list(FILTER _tmp_files EXCLUDE REGEX "(qasc|moc)_.+")
            list(APPEND _src_files ${_tmp_files})
            unset(_tmp_files)

            get_target_property(_tmp_dirs ${_item} INCLUDE_DIRECTORIES)
            list(APPEND _include_dirs ${_tmp_dirs})
        endforeach()
    endif()

    if(_src_files)
        if(NOT FUNC_LOCALES)
            message(FATAL_ERROR "ck_add_translations: source files collected but LOCALES not specified!")
        endif()
    elseif(NOT FUNC_TS_FILES)
        message(FATAL_ERROR "ck_add_translations: no source files or ts files collected!")
    endif()

    # Find linguist tools
    ck_find_qt_module(LinguistTools)

    add_custom_target(${_target})

    set(_qm_depends)

    if(FUNC_TS_OPTIONS)
        set(_ts_options ${FUNC_TS_OPTIONS})
    endif()

    if(FUNC_QM_OPTIONS)
        set(_qm_options ${FUNC_QM_OPTIONS})
    endif()

    if(_src_files)
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

        set(_ts_files)

        foreach(_loc ${FUNC_LOCALES})
            list(APPEND _ts_files ${_ts_dir}/${_prefix}_${_loc}.ts)
        endforeach()

        # Include options
        set(_include_options)

        foreach(_inc ${_include_dirs})
            list(APPEND _include_options "-I${_inc}")
        endforeach()

        # May be a lupdate bug, so we skip passing include directories
        # list(APPEND _ts_options ${_include_options})
        if(_ts_options)
            list(PREPEND _ts_options OPTIONS)
        endif()

        _ck_add_lupdate_target(${_target}_lupdate
            INPUT ${_src_files}
            OUTPUT ${_ts_files}
            ${_ts_options}
            CREATE_ONCE
        )

        add_dependencies(${_target} ${_target}_lupdate)
        add_dependencies(ChorusKit_UpdateTranslations ${_target}_lupdate)

    # list(APPEND _qm_depends DEPENDS ${_target}_lupdate)
    else()
        if(FUNC_PREFIX)
            message(WARNING "ck_add_translations: no source files collected, PREFIX ignored")
        endif()

        if(FUNC_TS_DIR)
            message(WARNING "ck_add_translations: no source files collected, TS_DIR ignored")
        endif()

        if(FUNC_TS_TARGET)
            message(WARNING "ck_add_translations: no source files collected, TS_TARGET ignored")
        endif()
    endif()

    if(FUNC_QM_DIR)
        set(_qm_dir ${FUNC_QM_DIR})
    else()
        set(_qm_dir ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    set(_qm_target)

    if(_qm_options)
        list(PREPEND _qm_options OPTIONS)
    endif()

    _ck_add_lrelease_target(${_target}_lrelease
        INPUT ${_ts_files} ${FUNC_TS_FILES}
        DESTINATION ${_qm_dir}
        ${_qm_options}
        ${_qm_depends}
    )

    add_dependencies(${_target} ${_target}_lrelease)
    add_dependencies(ChorusKit_CopySharedFiles ${_target}_lrelease)
    add_dependencies(ChorusKit_ReleaseTranslations ${_target}_lrelease)

    # Add release dependencies
    if(FUNC_TARGETS)
        foreach(_item ${FUNC_TARGETS})
            add_dependencies(${_item} ${_target}_lrelease)
        endforeach()
    endif()

    # ----------------- Template End -----------------
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

    if(NOT FUNC_PATTERNS)
        message(FATAL_ERROR "ck_add_files: PATTERNS not specified!")
    endif()

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
Install all headers in specified directory.

    ck_install_headers(_dir)
]] #
function(ck_install_headers _dir)
    if(NOT CK_ENABLE_DEVEL)
        return()
    endif()

    get_filename_component(_abs_dir ${_dir} ABSOLUTE)

    file(RELATIVE_PATH _rel_path ${CK_CMAKE_SOURCE_DIR} ${_abs_dir})
    set(_dest "${CK_INSTALL_INCLUDE_DIR}/${_rel_path}")
    install(DIRECTORY ${_abs_dir}/.
        DESTINATION "${_dest}"
        FILES_MATCHING REGEX ".+\\.(h|hpp)"
    )

    # Remove empty directories
    install(CODE "
        execute_process(
            COMMAND \"${CMAKE_COMMAND}\"
            -D \"dir=${CMAKE_INSTALL_PREFIX}/${_dest}\"
            -P \"${CK_CMAKE_SCRIPTS_DIR}/RemoveEmptyDirs.cmake\"
        )
    ")
endfunction()