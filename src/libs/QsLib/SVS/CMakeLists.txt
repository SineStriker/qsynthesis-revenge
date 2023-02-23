project(QsSVS
    VERSION 0.2.17.0
    LANGUAGES C CXX
)

get_subdirs(_dirs)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.SVS"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    QT_PRIVATE_INCLUDES Core
    LINKS QsBase
    LINKS_PRIVATE mrq
    INCLUDES ${_dirs}
)