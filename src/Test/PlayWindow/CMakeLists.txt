add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(PlayWindow
    VERSION 0.0.1.2
    SOURCES ${_src} res.qrc
    QT_LIBRARIES Widgets
    LINKS QsMedia
    INCLUDE_CURRENT
    WIN32_EXE
)
