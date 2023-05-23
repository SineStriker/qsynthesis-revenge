function(configure_library)
    ck_add_library(${PROJECT_NAME} AUTOGEN
        DESCRIPTION "ChorusKit Common Libraries"
        ${ARGN}
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_TRANSLATE TRUE
    )
endfunction()

add_subdirectory(appload)

add_subdirectory(appcore)

add_subdirectory(ikg2p)