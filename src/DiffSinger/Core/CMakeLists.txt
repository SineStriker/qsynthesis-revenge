project(DsCore LANGUAGES CXX VERSION ${SUB_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    LINKS QsSVS QsUtils
    INCLUDES include src
    WIN32_FILE_DESC "DiffSinger Core Module"
    WIN32_PRODUCT_NAME "DiffSinger.Core"
)
