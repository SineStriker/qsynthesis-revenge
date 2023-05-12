set(QT_CREATOR_VERSION 0.3.6.1)

set(QT_CREATOR_ENABLE_QML off)
set(QT_CREATOR_NO_FILENAME_LIST on)

function(configure_qtc_library _name _library_macro)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
        NAME ${_name}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "Qt Creator 3.6"
        COPYRIGHT "Copyright The Qt Company Ltd."
        LIBRARY_MACRO ${_library_macro}
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name}36)
    target_compile_definitions(${_name} PRIVATE _UNICODE UNICODE)
endfunction()

add_subdirectory(aggregation)
add_subdirectory(utils)
add_subdirectory(extensionsystem)

ck_install_headers(.)