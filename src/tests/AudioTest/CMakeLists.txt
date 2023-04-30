project(AudioTest VERSION 0.0.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

ck_add_executable(${PROJECT_NAME} SKIP_INSTALL)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework
)

ffmpeg_link_all(${PROJECT_NAME} PRIVATE)