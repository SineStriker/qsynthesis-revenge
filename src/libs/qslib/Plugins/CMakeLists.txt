set(CURRENT_PLUGIN_CATEGORY defaultplugins)
set(CURRENT_PLUGIN_DESC "QSynthesis Library Plugin")

function(configure_plugin _author _out)
    set(_target ${SUB_PROJECT}_Plugin_${PROJECT_NAME})
    ck_add_plugin(${_target}
        AUTHOR_NAME ${_author}
        FILE_DESC ${CURRENT_PLUGIN_DESC}
        PRODUCT_NAME ${PROJECT_NAME}
        PARENT ${SUB_PROJECT}
        SUBDIR plugins/${CURRENT_PLUGIN_CATEGORY}
        OUTPUT_NAME ${PROJECT_NAME}
    )
    set(${_out} ${_target} PARENT_SCOPE)
endfunction()

add_subdirectory(AudioDecoder)
add_subdirectory(AudioEncoder)
add_subdirectory(AudioPlayback)
add_subdirectory(WindowHandle)