project(qustx
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

file(GLOB _src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(_main_dirs Coders Ustx Util)
foreach(_dir ${_main_dirs})
    set(_temp)
    file(GLOB_RECURSE _temp ${_dir}/*.h ${_dir}/*.cpp)
    list(APPEND _src ${_temp})
endforeach()

add_library(${PROJECT_NAME} SHARED ${_src})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC QsSVS)
target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

if (WIN32)
    # Specify your plugin metadata
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    set(PROJECT_AUTHOR "Sine Striker")
    set(PROJECT_DESC "QSynthesis Plugin for Project Convertion")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME ${PROJECT_NAME})

    configure_file(${PLUGIN_WINRC_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

settle_plugin(${PROJECT_NAME} svsconverters)
