project(${SUB_PROJECT}
    LANGUAGES CXX
    VERSION ${SUB_VERSION}
)

# ------------------------------ Configure Target Begin ------------------------------

# Add executable
qs_add_executable(${PROJECT_NAME} LvStaging EXTERN_C)

# Set Properties
if((APP_DEPLOY AND NOT CONFIG_WIN32_DEBUG) OR NOT DEFINED NOT_QT_CREATOR)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Add embedded resources
if(WIN32)
    if(NOT TRUE)
        set(_manifest_desc)
    else()
        set(_manifest_desc "1 24 app.manifest")

        # configure manifest
        set(WIN32_MF_APPLICATION_IDENTIFIER "${SUB_IDENTIFIER}")
        set(WIN32_MF_APPLICATION_DESC "${SUB_DESCRIPTION}")
        configure_file(
            ${WIN32_EXE_MANIFEST}
            ${CMAKE_CURRENT_BINARY_DIR}/app.manifest
            @ONLY
        )

        target_sources(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/app.manifest
        )
    endif()

    # configure rc
    set(WIN32_ICON_NAME "app.ico")
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "OpenVPI Team")
    set(WIN32_FILE_DESC ${PROJECT_NAME})
    set(WIN32_PRODUCT_NAME ${PROJECT_NAME})
    # set(WIN32_RC_EXTRA_CONTENT "${_manifest_desc}")
    configure_file(
        ${WIN32_EXE_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )

    # copy ico
    file(COPY res/win32/app.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # add files
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
    )
elseif(APPLE)
    # copy files
    get_filename_component(MACOSX_BUNDLE_PLIST_FILENAME ${MACOSX_BUNDLE_PLIST} NAME)

    file(COPY ${MACOSX_BUNDLE_PLIST} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY res/mac/qsynth2.icns DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # configure mac plist
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/${MACOSX_BUNDLE_PLIST_FILENAME}
        MACOSX_BUNDLE_EXECUTABLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_INFO_STRING ${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE qsynth2.icns
        MACOSX_BUNDLE_GUI_IDENTIFIER com.OpenVPI.LabelVoice
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_COPYRIGHT "Apache 2.0 License"
    )
else()
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    TC_TARGET_TYPE EXECUTABLE
    TC_EXECUTABLE_TYPE Main
)

# ------------------------------ Configure Target End ------------------------------
