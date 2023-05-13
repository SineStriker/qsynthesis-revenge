project(CkAppCore
    VERSION ${CHORUSKIT_VERSION}
    LANGUAGES CXX
)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

configure_library(
    SHARED AUTOGEN
    VENDOR "Sine Striker"
)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    QT_INCLUDES_PRIVATE Core Gui Widgets
    LINKS QsFramework ExtensionSystem qBreakpad
    LINKS_PRIVATE SingleApplication
    INCLUDE_PRIVATE include/CoreApi include/CoreControls src
    AUTO_INCLUDE_DIRS include
)