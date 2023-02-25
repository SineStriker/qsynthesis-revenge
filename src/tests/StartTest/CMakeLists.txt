project(StartTest VERSION 0.0.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_files(_src CURRENT PATTERNS *.h *.cpp)

add_executable(${PROJECT_NAME})

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS QsFramework
)
