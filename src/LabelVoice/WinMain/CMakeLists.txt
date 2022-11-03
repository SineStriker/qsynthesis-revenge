project(${INNER_PROJECT}
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

add_executable(${PROJECT_NAME} main.cpp)

# Link OS dependent libraries
if(WIN32)
    # generate rc
    set(WIN32_ICON_NAME "app.ico")
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "OpenVPI Team")
    set(WIN32_FILE_DESC "LabelVoice")
    set(WIN32_PRODUCT_NAME ${PROJECT_NAME})
    configure_file(
        ${WIN32_EXE_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    # copy ico
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/win32/app.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    # add files
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
elseif(APPLE)
    # copy files
    get_filename_component(MACOSX_INFO_PLIST_FILENAME ${MACOSX_INFO_PLIST} NAME)
    
    file(COPY ${MACOSX_INFO_PLIST} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mac/qsynth2.icns DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # configure project
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/${MACOSX_INFO_PLIST_FILENAME}
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

# Set Properties
if(APP_DEPLOY OR NOT DEFINED NOT_QT_CREATOR)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if (APP_DEPLOY)
    add_dependencies(${PROJECT_NAME} LvStaging)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE LvStaging)
endif()

if (WIN32 AND APP_DEPLOY)
    get_target_property(_dll_name LvStaging OUTPUT_NAME)

    target_compile_definitions(${PROJECT_NAME} PRIVATE DELAY_LOAD)
    if (MSVC)
        target_compile_definitions(${PROJECT_NAME} PRIVATE APP_DLL="${_dll_name}.dll")
    else()
        target_compile_definitions(${PROJECT_NAME} PRIVATE APP_DLL="lib${_dll_name}.dll")
    endif()
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_LIB_DIR="${APP_LIB_DIR}")
