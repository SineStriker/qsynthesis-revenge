project(QsMedia
    VERSION 0.0.1.0
    LANGUAGES CXX
)

set(_dirs Api Global NAudio)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.Media"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    QT_PRIVATE_INCLUDES Core
    LINKS QsBase
    INCLUDES ${_dirs}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    TC_TARGET_TYPE LIBRARY
    TC_LIBRARY_TYPE ${SUB_PROJECT}
    TC_QT_BINARY true
)
