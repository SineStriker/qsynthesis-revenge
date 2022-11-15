project(SDLPlayback
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsMedia
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "QSynthesis Plugin for Audio Playback"
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
