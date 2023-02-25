project(SDLPlayback
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker" _plugin)

target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsMedia
)

target_link_libraries(${_plugin} PRIVATE SDL2::SDL2)
