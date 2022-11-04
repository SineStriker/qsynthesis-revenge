add_files(_src
    CURRENT
)

add_test_target(SpekTest
    VERSION 0.0.1.2
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    LIBRARIES QsMedia
    INCLUDE_CURRENT
)

ffmpeg_link_libraries(${PROJECT_NAME} PRIVATE avcodec avformat avutil swresample)

