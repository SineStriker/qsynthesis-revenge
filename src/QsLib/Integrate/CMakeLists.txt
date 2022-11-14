project(QsIntegrate
    VERSION 0.0.1.0
    LANGUAGES CXX
)

add_files(_src
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

qslib_configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Widgets
    QT_PRIVATE_INCLUDES Core Widgets
    LINKS QsFramework QsMedia QsSVS
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Common Utilities"
    WIN32_PRODUCT_NAME "QSynthesis.Integrate"
)
