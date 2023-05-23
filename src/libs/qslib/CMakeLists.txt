project(QsLib
    LANGUAGES CXX
    VERSION 0.2.18.1
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})

function(configure_library)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
        DESCRIPTION "QSynthesis Library"
        VENDOR "QSynthesis Team"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_TRANSLATE TRUE
    )
    ck_add_deploy_targets(${PROJECT_NAME})

    ck_install_headers()
endfunction()

add_subdirectory(SVS)
add_subdirectory(Media)
add_subdirectory(Framework)
add_subdirectory(Plugins)