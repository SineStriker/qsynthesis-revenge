project(NoteCombiner
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

file(GLOB _src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(${PROJECT_NAME} SHARED ${_src})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC QsSVS)

if (WIN32)
    # Specify your plugin metadata
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    set(PROJECT_AUTHOR "Sine Striker")
    set(PROJECT_DESC "QSynthesis Plugin for Note Combination")
    set(PROJECT_COPYRIGHT_START_YEAR "2020")
    set(PROJECT_COPYRIGHT_END_YEAR "2022")
    set(PROJECT_PRODUCT_NAME ${PROJECT_NAME})

    configure_file(${PLUGIN_WINRC_FILE}
        ${PROJECT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR}/res.rc)
endif()

settle_plugin(${PROJECT_NAME} utaplugins)
