project(SDLPlayback
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsMedia
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
