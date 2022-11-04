add_files(_src
    CURRENT
)

add_test_target(PipeTest
    SOURCES ${_src}
    QT_LIBRARIES Widgets Network Multimedia
    LIBRARIES QsMedia
    INCLUDE_CURRENT
)
