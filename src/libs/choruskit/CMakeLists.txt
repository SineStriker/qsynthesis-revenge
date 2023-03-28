set(CHORUSKIT_VERSION ${APP_VERSION_VERBOSE})
set(CHORUSKIT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(configure_ck_library _name)
    ck_target_res(${_name}
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "ChorusKit Common Libraries"
        COPYRIGHT "${ROOT_OPENVPI_COPYRIGHT}"
    )
    target_include_directories(${PROJECT_NAME} PUBLIC ${CHORUSKIT_LIB_DIR})
endfunction()

add_subdirectory(appload)

add_subdirectory(appcore)