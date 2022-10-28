cmake_minimum_required(VERSION 3.5)

project(PlayWindow LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

add_executable(PlayWindow
    ${PROJECT_SOURCES}
)

target_link_libraries(PlayWindow PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
