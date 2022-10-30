cmake_minimum_required(VERSION 3.5)

project(PipeTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})
