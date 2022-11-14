project(${INNER_PROJECT}
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

# ------------------------------ Configure Target Begin ------------------------------

# Add executable
add_executable(${PROJECT_NAME} main.cpp)

# Set Properties
if(APP_DEPLOY OR NOT DEFINED NOT_QT_CREATOR)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Add definitions
if(WIN32 AND APP_DEPLOY)
    add_dependencies(${PROJECT_NAME} LvStaging)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DELAY_LOAD)
    target_compile_definitions(${PROJECT_NAME} PRIVATE APP_DLL="$<TARGET_FILE_NAME:LvStaging>")
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE LvStaging)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_LIB_DIR="${APP_LIB_DIR}")

# Add embedded resources
if(WIN32)
    # configure rc
    set(WIN32_ICON_NAME "app.ico")
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "OpenVPI Team")
    set(WIN32_FILE_DESC "LabelVoice")
    set(WIN32_PRODUCT_NAME ${PROJECT_NAME})
    set(WIN32_MANIFEST_DESC "1 24 \"xpstyle.manifest\"")
    configure_file(
        ${WIN32_EXE_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )

    # copy ico
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/win32/app.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/win32/xpstyle.manifest DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # add files
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        # ${CMAKE_CURRENT_BINARY_DIR}/xpstyle.manifest
    )
elseif(APPLE)
    # copy files
    get_filename_component(MACOSX_BUNDLE_PLIST_FILENAME ${MACOSX_BUNDLE_PLIST} NAME)

    file(COPY ${MACOSX_BUNDLE_PLIST} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mac/qsynth2.icns DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

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
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    TC_TARGET_TYPE EXECUTABLE
    TC_EXECUTABLE_TYPE Main
)

# ------------------------------ Configure Target End ------------------------------
