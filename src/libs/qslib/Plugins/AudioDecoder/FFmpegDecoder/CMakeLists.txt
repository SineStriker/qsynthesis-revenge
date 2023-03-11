project(FFmpegDecoder
    VERSION 0.0.0.1
    LANGUAGES CXX
)

ck_add_files(_src
    CURRENT
    DIRS private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker" _plugin)

ck_target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core
    LINKS QsMedia
)

ffmpeg_link_libraries(${_plugin} PRIVATE avcodec avformat avutil swresample)
