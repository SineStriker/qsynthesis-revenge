#[[
Add plugin metadata of `PROJECT_NAME` and `PROJECT_VERSION`

    configure_plugin_base(<VAR>
        METADATA_IN <plugin.json.in>
        [COMPAT_VERSION     <version>]
        [SUBDIR_NAME        <dirname>]
        [PLUGIN_NAME        <name>]
    )

    VAR: Return the real target name (Mangled to avoid duplication)

    Arguments:
        COMPAT_VERSION: compatible version, default to 0.0.0.0
        SUBDIR_NAME: subdirectory name in plugins dir, default to `PROJECT_NAME`
        PLUGIN_NAME: plugin output name, default to `PROJECT_NAME`
]] #
function(configure_plugin_base _out)
    set(options)
    set(oneValueArgs METADATA_IN COMPAT_VERSION SUBDIR_NAME PLUGIN_NAME)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_target ${CURRENT_SUB_PROJECT}_Plugin_${PROJECT_NAME})
    ck_add_library(${_target} SHARED AUTOGEN ${FUNC_UNPARSED_ARGUMENTS})
    add_library(${CURRENT_SUB_PROJECT}::${PROJECT_NAME} ALIAS ${_target})

    if(FUNC_COMPAT_VERSION)
        set(_compat_version ${FUNC_COMPAT_VERSION})
    else()
        set(_compat_version 0.0.0.0)
    endif()

    if(FUNC_SUBDIR_NAME)
        set(_subdir_name ${FUNC_SUBDIR_NAME})
    else()
        set(_subdir_name ${PROJECT_NAME})
    endif()

    if(FUNC_PLUGIN_NAME)
        set(_name ${FUNC_PLUGIN_NAME})
    else()
        set(_name ${PROJECT_NAME})
    endif()

    # Set output directory
    set(_out_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CURRENT_SUB_PROJECT}/plugins/${_subdir_name})
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_out_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_out_dir}
        OUTPUT_NAME ${_name}
    )

    set(_plugin_json2_path ${CMAKE_CURRENT_BINARY_DIR}/${_name}PluginMetaData/plugin.json)
    file(GENERATE OUTPUT ${_plugin_json2_path} CONTENT "{\n    \"name\": \"${_name}\"\n}")

    add_custom_command(
        TARGET ${_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${_plugin_json2_path} ${_out_dir}
    )

    # Set plugin metadata
    set(PLUGIN_METADATA_VERSION
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}_${PROJECT_VERSION_TWEAK})
    ck_parse_version(_compat ${_compat_version})
    set(PLUGIN_METADATA_COMPAT_VERSION ${_compat_1}.${_compat_2}.${_compat_3}_${_compat_4})
    configure_file(
        ${FUNC_METADATA_IN}
        ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata/plugin.json
        @ONLY
    )
    target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata)

    set(${_out} ${_target} PARENT_SCOPE)
endfunction()

add_subdirectory(diffscope)