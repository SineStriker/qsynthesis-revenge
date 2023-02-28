project(CKAppLoad
    LANGUAGES CXX
    VERSION 0.0.0.1
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME "OpenVPI Team"
    FILE_DESC "ChorusKit Application Loader"
    PRODUCT_NAME "ChorusKit.AppLoader"
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} res.qrc
    QT_LIBRARIES Core Widgets
    LINKS QsFramework ExtensionSystem qBreakpad
    INCLUDES ${COMMON_INCLUDE_DIR}
)