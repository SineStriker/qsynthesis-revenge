add_files(_src
    CURRENT
)

add_test_target(UstxTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LIBRARIES QsSVS yaml-cpp
    INCLUDE_CURRENT
)
