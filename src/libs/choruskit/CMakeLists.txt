set(CHORUSKIT_VERSION ${CONFIG_APP_VERSION})

function(configure_ck_library _name)
    ck_target_res(${_name}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "ChorusKit Common Libraries"
        COPYRIGHT "${ROOT_OPENVPI_COPYRIGHT}"
    )
endfunction()

add_subdirectory(appload)

add_subdirectory(appcore)