add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(ZlibTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsUtils
    INCLUDE_CURRENT
)
