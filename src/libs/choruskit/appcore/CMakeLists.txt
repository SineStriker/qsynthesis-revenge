project(CkAppCore
    VERSION ${CHORUSKIT_VERSION}
    LANGUAGES CXX
)

set(_extra_includes)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem qBreakpad
    LINKS_PRIVATE SingleApplication
    INCLUDES ${_extra_includes}
    INCLUDES_PRIVATE include/CoreApi include/CoreControls src
    AUTO_INCLUDE_DIRS include
)

ck_install_headers(.)

configure_ck_library(${PROJECT_NAME})