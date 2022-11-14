include(Template.cmake)

if(NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(AudioDecoder)
    add_subdirectory(AudioEncoder)
    add_subdirectory(AudioPlayback)
    add_subdirectory(CompressEngine)
    add_subdirectory(WindowFactory)
endif()

if(ENABLE_UTA_PLUGINS)
    add_subdirectory(OtoPlugin)
    add_subdirectory(UtaPlugin)
endif()

if(ENABLE_SVS_CONVERTERS)
    add_subdirectory(SVSConverter)
endif()
