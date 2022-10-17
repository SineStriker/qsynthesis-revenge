include(${PROJECT_SCRIPTS_DIR}/Modules/Basic.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/FindQt.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/ListUtil.cmake)

# ------------------------------ Basic information ------------------------------

project(QSynthesis
    VERSION 0.0.0.1
    LANGUAGES CXX
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
set(INCLUDE_PATHS)

# ------------------------------ Include/Import/Link ------------------------------
add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)

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

list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# Add executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES} ${MAC_RES})

# Add definitions
add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")

# Link OS dependent libraries
if(SYS_WIN)
    # generate rc
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/win32/res.rc.in
        ${PROJECT_BINARY_DIR}/res.rc
        @ONLY
    )
    # copy ico
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Resources/platform/win32/app.ico DESTINATION ${PROJECT_BINARY_DIR})
    # add files
    set(RC_FILES ${PROJECT_BINARY_DIR}/res.rc)
    target_sources(${PROJECT_NAME} PRIVATE ${RC_FILES})
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
if (USE_FRAMELESS)
    target_link_libraries(${PROJECT_NAME} PRIVATE FramelessHelper::Widgets)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC NO_FRAMELESS_HELPER)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE QsUtils QsFramework QsMedia QsSVS)

target_link_libraries(${PROJECT_NAME} PRIVATE QuaZip::QuaZip)

target_link_libraries(${PROJECT_NAME} PRIVATE fsnotifier-lib)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

# Add include dirs
target_include_directories(${PROJECT_NAME} PRIVATE
    Core
    Staging
)
