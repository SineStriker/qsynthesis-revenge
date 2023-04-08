set(QT_CREATOR_VERSION 0.3.6.1)

set(QT_CREATOR_ENABLE_QML off)
set(QT_CREATOR_NO_FILENAME_LIST on)

function(configure_qtc_library _name)
    ck_target_res(${_name}
        NAME QTC${_name}36
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "Qt Creator 3.6"
        COPYRIGHT "Copyright The Qt Company Ltd."
    )
    target_compile_definitions(${_name} PRIVATE _UNICODE)
    target_compile_definitions(${_name} PRIVATE UNICODE)
endfunction()

add_subdirectory(aggregation)
add_subdirectory(utils)
add_subdirectory(extensionsystem)