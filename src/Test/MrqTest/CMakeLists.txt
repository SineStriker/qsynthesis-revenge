add_files(_src
    CURRENT
)

add_test_target(MrqTest
    SOURCES ${_src}
    QT_LIBRARIES Core
    LIBRARIES QsSVS
    INCLUDE_CURRENT
)
