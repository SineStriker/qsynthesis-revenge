add_files(_src
    CURRENT
)

add_test_target(DecodeTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LIBRARIES QsMedia
    INCLUDE_CURRENT
)
