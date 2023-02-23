set(QT_CREATOR_VERSION 0.3.16.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    add_compile_definitions(_UNICODE)
    add_compile_definitions(UNICODE)
endif()

function(configure_qtc_library _name)
    qs_add_library(${_name}
        ENABLE_SHARED
        AUTHOR_NAME "The Qt Company Ltd."
        FILE_DESC "Qt Creator 3.16"
        PRODUCT_NAME "${_name}"
        COPYRIGHT_YEAR "(C) 2015"
        OUTPUT_NAME QTC3.6-${_name}
        LIBRARY_TYPE qtcreator
    )
endfunction()

add_subdirectory(aggregation)
add_subdirectory(utils)
add_subdirectory(extensionsystem)