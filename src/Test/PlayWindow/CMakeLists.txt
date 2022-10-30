cmake_minimum_required(VERSION 3.5)

project(PlayWindow
    VERSION 0.0.1.2
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CONSOLE_APP OFF)

set(QT_LIBS)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Network)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Multimedia REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Multimedia REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Multimedia)

file(GLOB _src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(_main_dirs)
foreach(_dir ${_main_dirs})
    set(_temp)
    file(GLOB_RECURSE _temp ${_dir}/*.h ${_dir}/*.cpp)
    list(APPEND _src ${_temp})
endforeach()

set(PROJECT_SOURCES
    ${_src}
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES} res.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

target_link_libraries(${PROJECT_NAME} PRIVATE QsMedia)

if (NOT CONSOLE_APP)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
    )
endif()
