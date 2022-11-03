include(${PROJECT_MODULES_DIR}/Link.cmake)

macro(settle_plugin _target _category)
    deploy_shared_libraries(
        TARGET ${_target}
        DESTINATION ${INNER_PLUGIN_DIR}/${_category}
        DEPENDENCIES ${_target}
        RELATIVE
    )
endmacro()

add_subdirectory(TemplateGenerator)
