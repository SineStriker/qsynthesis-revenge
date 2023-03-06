project(QsMedia
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

get_subdirs(_dirs)
set(_extra_includes)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_library(${PROJECT_NAME}
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
    LINKS QMCoreLib
    INCLUDES ${_dirs} ${_extra_includes}
)