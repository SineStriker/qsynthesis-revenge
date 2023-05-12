function(configure_library)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
        DESCRIPTION "ChorusKit Common Libraries"
        VENDOR "OpenVPI"
    )
endfunction()

add_subdirectory(appload)

add_subdirectory(appcore)

add_subdirectory(ikg2p)

ck_install_headers(.)