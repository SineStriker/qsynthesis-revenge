project(qmidi
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_subdirectory(QMidi)

file(GLOB _src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(${PROJECT_NAME} SHARED ${_src})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC QsSVS)
target_link_libraries(${PROJECT_NAME} PRIVATE QMidi)

if (WIN32)
    # Specify your plugin metadata
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "Sine Striker")
    set(WIN32_FILE_DESC "QSynthesis Plugin for Project Convertion")
    set(WIN32_PRODUCT_NAME ${PROJECT_NAME})
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

settle_plugin(${PROJECT_NAME} svsconverters)
