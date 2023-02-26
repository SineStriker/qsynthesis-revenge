project(${SUB_APP_LOADER}
    LANGUAGES CXX
    VERSION ${SUB_VERSION}
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffScope.Main"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS QsFramework ExtensionSystem qBreakpad
    INCLUDES ${COMMON_INCLUDE_DIR} ${_dirs}
)