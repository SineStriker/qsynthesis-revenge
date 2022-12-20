
add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(ReadAudioBenchmark
    VERSION 0.0.1.0
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    LINKS QsMedia
    INCLUDE_CURRENT
    WIN32_EXE
)
