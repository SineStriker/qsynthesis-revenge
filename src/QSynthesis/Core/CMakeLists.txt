project(QUtaCore LANGUAGES CXX VERSION ${SUB_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

if(WIN32)
    list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.(h|cpp|hpp|cc)")
elseif(APPLE)
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.(h|cpp|hpp|cc)")
else()
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.(h|cpp|hpp|cc)")
endif()

configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Core Module"
    WIN32_PRODUCT_NAME "QSynthesis.Core"
)
