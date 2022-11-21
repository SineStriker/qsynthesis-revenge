project(QUtaCore LANGUAGES CXX VERSION ${SUB_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

lvmod_configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Core Module"
    WIN32_PRODUCT_NAME "QSynthesis.Core"
)
