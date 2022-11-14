add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(DecodeTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsMedia
    INCLUDE_CURRENT
)
