set(CURRENT_PLUGIN_CATEGORY defaultplugins)
set(CURRENT_PLUGIN_DESC "DiffScope Plugin")

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

add_subdirectory(DsEngine)

add_subdirectory(DsPorter)