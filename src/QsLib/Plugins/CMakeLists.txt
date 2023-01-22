set(CURRENT_PLUGIN_CATEGORY defaultplugins)

function(configure_plugin _author)
    qs_add_plugin(${PROJECT_NAME}
        AUTHOR_NAME ${_author}
        FILE_DESC ${CURRENT_PLUGIN_DESC}
        PRODUCT_NAME ${PROJECT_NAME}
        PARENT ${SUB_PROJECT}
        SUBDIR ${SUB_PLUGIN_DIR}
        CATEGORY ${CURRENT_PLUGIN_CATEGORY}
    )
endfunction()

add_subdirectory(AudioDecoder)
add_subdirectory(AudioEncoder)
add_subdirectory(AudioPlayback)
add_subdirectory(CompressEngine)
add_subdirectory(SVSConverter)
add_subdirectory(WindowFactory)