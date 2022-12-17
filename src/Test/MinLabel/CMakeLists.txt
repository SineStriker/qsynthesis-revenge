add_files(_src
    CURRENT
    DIRECTORIES gui
    PATTERNS *.h *.cpp
)

add_test_target(MinLabel
    VERSION 0.0.1.5
    SOURCES ${_src} res.qrc
    QT_LIBRARIES Widgets
    LINKS QsMedia
    INCLUDE_CURRENT
    WIN32_EXE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSON="${PROJECT_VERSION}")
