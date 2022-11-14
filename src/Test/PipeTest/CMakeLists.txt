add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(PipeTest
    SOURCES ${_src}
    QT_LIBRARIES Widgets Network Multimedia
    LINKS QsMedia
    INCLUDE_CURRENT
)
