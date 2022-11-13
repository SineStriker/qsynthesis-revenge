include(${PROJECT_MODULES_DIR}/Basic.cmake)
include(${PROJECT_MODULES_DIR}/Find.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

project(LvStaging
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

# Library type
set(LVSTAGING_BUILD_STATIC OFF)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# ------------------------------ Configure Target Begin ------------------------------

# Add source files
set(QT_LIBS)
set(QT_INCS)
set(SOURCES)

add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)
add_qt_module(QT_LIBS Network)

add_qt_private_inc(QT_INCS Gui)
add_qt_private_inc(QT_INCS Widgets)

set(_main_dirs Kernel Managers Windows)
foreach(_dir ${_main_dirs})
    list_add_recursively(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cc
    )
endforeach()
list(APPEND SOURCES lvstaging_global.h)

# Add library
if (LVSTAGING_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC LVSTAGING_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE LVSTAGING_LIBRARY)

# Set library properties
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
set(PROJECT_OUTPUT_NAME ${PROJECT_NAME_LOWER}-qt${QT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_OUTPUT_NAME}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC LvCore LvElem)
target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${QT_INCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add embedded resources
if(WIN32)
    # configure rc
    get_target_property(WIN32_EXPORT_NAME ${PROJECT_NAME} OUTPUT_NAME)
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "OpenVPI Team")
    set(WIN32_FILE_DESC "LabelVoice Main Module")
    set(WIN32_PRODUCT_NAME "LabelVoice.Staging")
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Resources/res.qrc)


# ------------------------------ Configure Target End ------------------------------

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
