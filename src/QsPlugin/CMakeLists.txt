include(${PROJECT_SCRIPTS_DIR}/Modules/Link.cmake)

macro(settle_plugin _target _category)
    deploy_shared_libraries(
        TARGET ${_target}
        DESTINATION ${APP_PLUGINS_DIR}/${_category}
        DEPENDENCIES ${_target}
        RELATIVE
    )
endmacro()

set(PLUGIN_WINRC_FILE ${WIN32_DLL_RC})

if (NOT BUILD_SVS_PLUGINS_ONLY)

    add_subdirectory(AudioDecoder)

    add_subdirectory(AudioEncoder)

    add_subdirectory(AudioPlayback)

    add_subdirectory(CompressEngine)

    add_subdirectory(WindowFactory)

endif()

if (ENABLE_UTA_PLUGINS)
    add_subdirectory(OtoPlugin)
    add_subdirectory(UtaPlugin)
endif()

if (ENABLE_SVS_CONVERTERS)
    add_subdirectory(SVSConverter)
endif()
