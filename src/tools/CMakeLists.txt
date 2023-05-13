if(LINUX AND NOT QT_DEPLOY_EXECUTABLE)
    add_subdirectory(linuxdeployqt)
    set_target_properties(linuxdeployqt
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CK_GLOBAL_RUNTIME_OUTPUT_PATH}
    )
    set(QT_DEPLOY_EXECUTABLE $<TARGET_FILE:${linuxdeployqt}>)
endif()