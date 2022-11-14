project(LvStaging
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

add_files(_src
    CURRENT
    DIRECTORIES Kernel Managers Windows
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_lv_mod(
    SOURCES ${_src} Resources/res.qrc
    ENABLE_SHARED
    QT_LIBRARIES Widgets Svg
    QT_PRIVATE_INCLUDES Gui Widgets
    LINKS LvCore LvElem
    INCLUDE_CURRENT
    WIN32_FILE_DESC "LabelVoice Main Module"
    WIN32_PRODUCT_NAME "LabelVoice.Staging"
)

# Add depleploy commands
if (WIN32 AND APP_DEPLOY)

    # Deploy Qt
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

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_DEPLOY_EXECUTABLE}"
        --libdir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${APP_LIB_DIR}"
        --plugindir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${APP_PLUGINS_DIR}"
        --no-translations
        --no-system-d3d-compiler
        --no-virtualkeyboard
        --no-compiler-runtime
        --no-opengl-sw
        --force
        --verbose 0
        "$<TARGET_FILE:${PROJECT_NAME}>"
    )

    deploy_shared_libraries(
        TARGET ${PROJECT_NAME}
        DESTINATION ${APP_LIB_DIR}
        DEPENDENCIES FFmpeg::avcodec FFmpeg::avformat FFmpeg::avutil FFmpeg::swresample
                     SDL2::SDL2 yaml-cpp
                     FramelessHelper::FramelessHelperCore FramelessHelper::FramelessHelperWidgets
                     QuaZip::QuaZip # ZLIB::zlib
        RELATIVE PREBUILT
    )

    deploy_shared_libraries(
        TARGET ${PROJECT_NAME}
        DESTINATION ${APP_LIB_DIR}
        DEPENDENCIES ${PROJECT_NAME} LvCore LvElem QsUtils
                                     QsIntegrate QsFramework QsSVS QsMedia fsnotifier-qt
        RELATIVE
    )

    deploy_shared_libraries(
        TARGET ${PROJECT_NAME}
        DESTINATION ${APP_LIB_DIR}
        DEPENDENCIES Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui
                     Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg
                     Qt${QT_VERSION_MAJOR}::Network
        RELATIVE PREBUILT
    )

    if (MSVC)
        # ZLIB's package file doesn't provide dll information
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/zlib1.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/${APP_LIB_DIR}
        )
    endif()

endif()
