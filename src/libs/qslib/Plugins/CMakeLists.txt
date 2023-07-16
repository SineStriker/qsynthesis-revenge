set(CURRENT_PLUGIN_CATEGORY defaultplugins)
set(CURRENT_PLUGIN_DESC "QSynthesis Library Plugin")

# Use `PROJECT_NAME` as name and `PROJECT_VERSION` as version by default
function(configure_plugin _plugin _author)
    set(_target QsLib__${PROJECT_NAME})
    ck_add_library_plugin(${_target} ${CURRENT_PLUGIN_CATEGORY}
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION ${CURRENT_PLUGIN_DESC}
        VENDOR ${_author}
        SKIP_EXPORT
    )
    set(${_plugin} ${_target} PARENT_SCOPE)
    set_target_properties(${_target} PROPERTIES
        QT_TRANSLATE TRUE
    )
endfunction()

add_subdirectory(AudioDecoder)
add_subdirectory(AudioEncoder)
add_subdirectory(AudioPlayback)
add_subdirectory(WindowHandle)
add_subdirectory(IconEngine)