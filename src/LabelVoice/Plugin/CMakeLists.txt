include(${PROJECT_SCRIPTS_DIR}/Modules/Link.cmake)

macro(settle_plugin _target _category)
    deploy_shared_libraries(
        TARGET ${_target}
        DESTINATION ${INNER_PLUGIN_DIR}/${_category}
        DEPENDENCIES ${_target}
        RELATIVE
    )
endmacro()

set(PLUGIN_WINRC_FILE ${WIN32_DLL_RC})

add_subdirectory(TemplateGenerator)
