cmake_minimum_required(VERSION 3.5)

project(SpekTest
    VERSION 0.0.1.1
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CONSOLE_APP ON)

set(QT_LIBS)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

file(GLOB _src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

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

ffmpeg_link_libraries(${PROJECT_NAME} PRIVATE avcodec avformat avutil swresample)

if (NOT CONSOLE_APP)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
