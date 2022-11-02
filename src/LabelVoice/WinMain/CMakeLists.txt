project(${INNER_PROJECT}
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

add_executable(${PROJECT_NAME} main.cpp)

# Link OS dependent libraries
if(WIN32)
    # generate rc
    set(PROJECT_ICON_FILENAME "app.ico")
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    set(PROJECT_AUTHOR "OpenVPI Team")
    set(PROJECT_DESC "LabelVoice")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME ${PROJECT_NAME})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/win32/res.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    # copy ico
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/win32/app.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    # add files
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
elseif(APPLE)
    set(MAC_PINFO_LIST_TEMPLATE MacOSXBundleInfo.plist.in)

    # copy files
    find_file(INFO_PLIST_FILE ${MAC_PINFO_LIST_TEMPLATE} ${CMAKE_MODULE_PATH})
    file(COPY ${INFO_PLIST_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mac/qsynth2.icns DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # configure project
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/${MAC_PINFO_LIST_TEMPLATE}
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
