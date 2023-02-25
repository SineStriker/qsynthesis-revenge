include_guard(DIRECTORY)

function(proc_deploy2 _all_targets)
    # Add deploy target
    string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)

    set(_release_name ${PROJECT_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})
    set(_deploy_dir ${CMAKE_BINARY_DIR}/${_release_name})

    # Find Qt tools
    if(NOT DEFINED QT_QMAKE_EXECUTABLE)
        get_target_property(QT_QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
    endif()

    if(NOT EXISTS "${QT_QMAKE_EXECUTABLE}")
        message("Cannot find the QMake executable.")
        return()
    endif()

    get_filename_component(QT_BIN_DIRECTORY "${QT_QMAKE_EXECUTABLE}" DIRECTORY)

    find_program(QT_DEPLOY_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${QT_BIN_DIRECTORY}")

    if(NOT EXISTS "${QT_DEPLOY_EXECUTABLE}")
        message("Cannot find the deployqt tool.")
        return()
    endif()

    set(_target_files)

    foreach(_item ${_all_targets})
        get_target_property(_type ${_item} TYPE)

        if(_type MATCHES "(EXECUTABLE|SHARED_LIBRARY|STATIC_LIBRARY)")
            list(APPEND _target_files $<TARGET_FILE:${_item}>)
        endif()
    endforeach()

    # Add deploy target
    add_custom_target(deploy DEPENDS all
        COMMAND ${CMAKE_COMMAND}
        -D "targets=${_target_files}"
        -D "deployqt=${QT_DEPLOY_EXECUTABLE}"
        -D "orgdir=${CONFIG_OUTPUT_DIRECTORY}"
        -D "newdir=${_deploy_dir}"
        -D "dummy=$<TARGET_FILE:qtdummy>"
        -P "${PROJECT_MODULES_DIR}/../DeployHelper.cmake"
    )
endfunction()