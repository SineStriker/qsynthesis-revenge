add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_test_target(UstxTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsSVS yaml-cpp
    INCLUDE_CURRENT
)
