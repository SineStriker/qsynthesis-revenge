include(${PROJECT_MODULES_DIR}/Basic.cmake)
include(${PROJECT_MODULES_DIR}/Find.cmake)
include(${PROJECT_MODULES_DIR}/Link.cmake)
include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

project(LvElem
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

# Library type
set(LVELEM_BUILD_STATIC OFF)

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

set(_main_dirs Controls Kernel Managers Styles)
foreach(_dir ${_main_dirs})
    list_add_recursively(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/*.cc
    )
endforeach()
list(APPEND PROJECT_SOURCES lvelem_global.h)

# Add library
if (LVELEM_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC LVELEM_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE LVELEM_LIBRARY)

# Set library properties
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
set(PROJECT_OUTPUT_NAME ${PROJECT_NAME_LOWER}-qt${QT_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_OUTPUT_NAME}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC QsFramework)
target_link_libraries(${PROJECT_NAME} PUBLIC LvCore)
target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})

# Add definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC APP_VERSION="${PROJECT_VERSION_SHORT}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_PLUGINS_DIR="${APP_PLUGINS_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_RES_DIR="${APP_RES_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_RES_PLUGINS_DIR="${APP_RES_PLUGINS_DIR}")

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
    set(WIN32_FILE_DESC "LabelVoice Elementary Module")
    set(WIN32_PRODUCT_NAME "LabelVoice.Element")
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

# ------------------------------ Configure Target End ------------------------------
