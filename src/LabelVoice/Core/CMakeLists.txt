include(${PROJECT_SCRIPTS_DIR}/Modules/Basic.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/Find.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/Link.cmake)
include(${PROJECT_SCRIPTS_DIR}/Modules/ListUtil.cmake)


# ------------------------------ Basic information ------------------------------

project(LvCore
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
set(_main_dirs Basic Commands Render Serialization Utils)
foreach(_dir ${_main_dirs})
    list_add_recursively(PROJECT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cc)
    list_add_recursively(PROJECT_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.h)
endforeach()
list(APPEND PROJECT_SOURCES lvcore_global.h)

# Add library
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
set(PROJECT_OUTPUT_NAME ${PROJECT_NAME_LOWER}-qt${QT_VERSION_MAJOR})

if (LVCORE_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC LVCORE_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE LVCORE_LIBRARY)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_OUTPUT_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# OS Specific
if(WIN32)
    # generate rc
    get_target_property(PROJECT_EXPORT_FILENAME ${PROJECT_NAME} OUTPUT_NAME)
    set(PROJECT_AUTHOR "OpenVPI Team")
    set(PROJECT_DESC "LabelVoice Core Module")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME "LabelVoice.Core")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Staging/Resources/win32/res.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE QsUtils QsFramework QsSVS)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

# Add definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${APP_VERSION_SHORT}")
