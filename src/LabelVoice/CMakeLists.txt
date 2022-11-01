include(${PROJECT_SCRIPTS_DIR}/Modules/Basic.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/FindQt.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/Link.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/ListUtil.cmake)

# ------------------------------ Basic information ------------------------------

project(LabelVoice
    VERSION 0.0.1.2
    LANGUAGES CXX
)

# Set app data
set(APP_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(APP_VERSION_SHORT ${APP_VERSION_SHORT} PARENT_SCOPE)
set(APP_VERSION_VERBOSE ${PROJECT_VERSION} PARENT_SCOPE)

set(APP_BLOCK LvBlock)
set(APP_LIB_DIR lib)

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
set(INCLUDE_PATHS)

# ------------------------------ Include/Import/Link ------------------------------
add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)
add_qt_module(QT_LIBS Network)

include_qt_private(Gui)
include_qt_private(Widgets)

# Add fixed files
set(_main_dirs Core Staging Vendor)
foreach(_dir ${_main_dirs})
    list_add_recursively(PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cc)
    list_add_recursively(PROJECT_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.h)
endforeach()

set(QT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/res.qrc)

# Add executable
string(TOLOWER ${APP_BLOCK} APP_BLOCK_LOWER)
set(BLOCK_OUTPUT_NAME ${APP_BLOCK_LOWER}-qt${QT_VERSION_MAJOR})
add_library(${APP_BLOCK} SHARED
    ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
set_target_properties(${APP_BLOCK} PROPERTIES
    OUTPUT_NAME ${BLOCK_OUTPUT_NAME}
)

add_executable(${PROJECT_NAME} main.cpp)

# Add definitions
target_compile_definitions(${APP_BLOCK} PRIVATE APP_VERSION="${APP_VERSION_SHORT}")

# Link OS dependent libraries
if(SYS_WIN)
    # generate rc
    set(PROJECT_ICON_FILENAME "app.ico")
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    set(PROJECT_AUTHOR "OpenVPI Team")
    set(PROJECT_DESC "LabelVoice")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME ${PROJECT_NAME})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/win32/res_exe.rc.in
        ${PROJECT_BINARY_DIR}/res.rc
        @ONLY
    )
    # copy ico
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/win32/app.ico DESTINATION ${PROJECT_BINARY_DIR})
    # add files
    target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR}/res.rc)

    # generate rc
    get_target_property(PROJECT_EXPORT_FILENAME ${APP_BLOCK} OUTPUT_NAME)
    set(PROJECT_DESC "LabelVoice Main Module")
    set(PROJECT_PRODUCT_NAME ${APP_BLOCK})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/win32/res_dll.rc.in
        ${PROJECT_BINARY_DIR}/res2.rc
        @ONLY
    )
    target_sources(${APP_BLOCK} PRIVATE ${PROJECT_BINARY_DIR}/res2.rc)

elseif(SYS_MAC)
    # configure files
    set(MAC_RES)
    foreach (Item ${MAC_RES})
        set_source_files_properties(${Item} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    endforeach()
    # configure project
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MACOSX_BUNDLE_INFO_PLIST
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/mac/Info.plist.in
    )
    # add files
    target_sources(${PROJECT_NAME} PRIVATE ${MAC_RES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE asound)
endif()

# Set Properties
if(APP_DEPLOY OR NOT DEFINED NOT_QT_CREATOR)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
    )
endif()

# Embedded libraries
target_link_libraries(${APP_BLOCK} PRIVATE QsUtils QsFramework QsSVS)

target_link_libraries(${APP_BLOCK} PRIVATE fsnotifier-lib)

target_link_libraries(${APP_BLOCK} PRIVATE yaml-cpp)

# Link Qt libraries
target_link_libraries(${APP_BLOCK} PRIVATE ${QT_LIBS})

# Add include dirs
target_include_directories(${APP_BLOCK} PRIVATE
    Core
)

target_include_directories(${APP_BLOCK} PUBLIC
    Staging
)

if (NOT APPLE)
    deploy_shared_libraries(
        TARGET ${PROJECT_NAME}
        DESTINATION ${APP_LIB_DIR}
        DEPENDENCIES FFmpeg::avcodec FFmpeg::avformat FFmpeg::avutil FFmpeg::swresample
                     SDL2::SDL2 yaml-cpp
                     FramelessHelper::FramelessHelperCore FramelessHelper::FramelessHelperWidgets
                     QuaZip::QuaZip # ZLIB::zlib
        RELATIVE PREBUILT
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE LIB_DIR="${APP_LIB_DIR}")
endif()

if (WIN32 AND APP_DEPLOY)
    add_dependencies(${PROJECT_NAME} ${APP_BLOCK})
    target_compile_definitions(${PROJECT_NAME} PRIVATE DELAY_LOAD)

    if (MSVC)
        target_compile_definitions(${PROJECT_NAME} PRIVATE APP_DLL="${BLOCK_OUTPUT_NAME}.dll")
    else()
        target_compile_definitions(${PROJECT_NAME} PRIVATE APP_DLL="lib${BLOCK_OUTPUT_NAME}.dll")
    endif()

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
        "$<TARGET_FILE:${APP_BLOCK}>"
    )

    deploy_shared_libraries(
        TARGET ${PROJECT_NAME}
        DESTINATION ${APP_LIB_DIR}
        DEPENDENCIES ${APP_BLOCK} QsUtils QsFramework QsSVS QsMedia fsnotifier-lib
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

else()

    target_link_libraries(${PROJECT_NAME} PRIVATE ${APP_BLOCK})

endif()
