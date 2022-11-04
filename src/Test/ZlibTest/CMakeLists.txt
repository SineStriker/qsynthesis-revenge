add_files(_src
    CURRENT
)

add_test_target(ZlibTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LIBRARIES QsUtils
    INCLUDE_CURRENT
)
