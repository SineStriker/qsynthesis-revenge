add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(MrqTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDE_CURRENT
)
