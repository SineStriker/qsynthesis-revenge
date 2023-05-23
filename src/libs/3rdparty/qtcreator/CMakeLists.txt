set(QT_CREATOR_VERSION 0.3.6.1)

set(QT_CREATOR_ENABLE_QML off)
set(QT_CREATOR_NO_FILENAME_LIST on)

function(configure_qtc_library _name)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
        NAME ${_name}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "Qt Creator 3.6"
        COPYRIGHT "Copyright The Qt Company Ltd."
        ${ARGN}
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${_name}36
        QT_TRANSLATE TRUE
        VENDOR qtcreator
    )
    target_compile_definitions(${_name} PRIVATE _UNICODE UNICODE)
    ck_add_deploy_targets(${PROJECT_NAME})
endfunction()

add_subdirectory(aggregation)
add_subdirectory(utils)
add_subdirectory(extensionsystem)

ck_install_headers()