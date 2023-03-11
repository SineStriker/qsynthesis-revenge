set(CURRENT_PLUGIN_CATEGORY defaultplugins)
set(CURRENT_PLUGIN_DESC "QSynthesis Library Plugin")

function(configure_plugin _author _out)
    set(_target ${SUB_PROJECT}_Plugin_${PROJECT_NAME})
    ck_add_library(${_target} SHARED AUTOGEN)
    add_library(${SUB_PROJECT}::${PROJECT_NAME} ALIAS ${_target})

    ck_target_res(${_target}
        DESCRIPTION ${CURRENT_PLUGIN_DESC}
        COPYRIGHT "Copyright ${_author}"
    )

    set(_out_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${SUB_PROJECT}/plugins/${CURRENT_PLUGIN_CATEGORY})
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_out_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_out_dir}
    )

    set(${_out} ${_target} PARENT_SCOPE)
endfunction()

add_subdirectory(AudioDecoder)
add_subdirectory(AudioEncoder)
add_subdirectory(AudioPlayback)
add_subdirectory(WindowHandle)