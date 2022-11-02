include(${PROJECT_SCRIPTS_DIR}/Modules/Basic.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/Find.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/Link.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/ListUtil.cmake)


# ------------------------------ Basic information ------------------------------

project(LvStaging
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

# Include current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Variables
set(QT_LIBS)
set(QT_RESOURCES)
set(PROJECT_SOURCES)
set(PROJECT_HEADERS)


# ------------------------------ Include/Import/Link ------------------------------

add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)
add_qt_module(QT_LIBS Network)

include_qt_private(Gui)
include_qt_private(Widgets)

# Add fixed files
set(_main_dirs Controls Kernel Managers Styles Windows)
foreach(_dir ${_main_dirs})
    list_add_recursively(PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cc)
    list_add_recursively(PROJECT_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.h)
endforeach()
list(APPEND PROJECT_SOURCES Entry.h Entry.cpp)

set(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/res.qrc)

# Add library
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
set(PROJECT_OUTPUT_NAME ${PROJECT_NAME_LOWER}-qt${QT_VERSION_MAJOR})

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_OUTPUT_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# OS Specific
if(WIN32)
    # generate rc
    get_target_property(PROJECT_EXPORT_FILENAME ${PROJECT_NAME} OUTPUT_NAME)
    set(PROJECT_AUTHOR "OpenVPI Team")
    set(PROJECT_DESC "LabelVoice Main Module")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME "LabelVoice.Staging")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/win32/res.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE QsUtils QsFramework QsSVS)

target_link_libraries(${PROJECT_NAME} PRIVATE LvCore)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

# Add definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${APP_VERSION_SHORT}")


# ------------------------------ Deploy ------------------------------

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
        --libdir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/lib"
        --plugindir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins"
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
        DEPENDENCIES ${PROJECT_NAME} LvCore QsUtils QsFramework QsSVS QsMedia fsnotifier-lib
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
