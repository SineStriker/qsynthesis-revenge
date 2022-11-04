add_files(_src
    CURRENT
)

add_test_target(PlayWindow
    VERSION 0.0.1.2
    SOURCES ${_src} res.qrc
    QT_LIBRARIES Widgets
    LIBRARIES QsMedia
    WIN32_EXE
    INCLUDE_CURRENT
)
