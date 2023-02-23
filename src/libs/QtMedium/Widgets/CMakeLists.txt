project(QMWidgets
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
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
    PRODUCT_NAME "QtMedium"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Widgets Svg
    QT_PRIVATE_INCLUDES Gui Widgets
    LINKS QMCoreLib
    INCLUDES ${_dirs}
)