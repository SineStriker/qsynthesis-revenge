set(QT_CREATOR_VERSION 0.3.6.1)
set(QT_CREATOR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(QT_CREATOR_ENABLE_QML off)
set(QT_CREATOR_NO_FILENAME_LIST on)

function(configure_qtc_library _name)
    qs_add_library(${_name}
        ENABLE_SHARED
        AUTHOR_NAME "The Qt Company Ltd."
        FILE_DESC "Qt Creator 3.6"
        PRODUCT_NAME "${_name}"
        COPYRIGHT_YEAR "(C) 2015"
        OUTPUT_NAME QTC${_name}36
        LIBRARY_TYPE qtcreator
    )
    target_compile_definitions(${_name} PRIVATE _UNICODE)
    target_compile_definitions(${_name} PRIVATE UNICODE)
    target_include_directories(${PROJECT_NAME} PUBLIC ${QT_CREATOR_LIB_DIR})
endfunction()

add_subdirectory(aggregation)
add_subdirectory(utils)
add_subdirectory(extensionsystem)