project(AudioTest VERSION 0.0.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

#ck_add_executable(${PROJECT_NAME} SKIP_INSTALL)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN SKIP_INSTALL)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework
)

ffmpeg_link_all(${PROJECT_NAME} PRIVATE)