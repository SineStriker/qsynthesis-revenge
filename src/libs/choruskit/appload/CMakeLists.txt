project(CkAppLoad
    LANGUAGES CXX
    VERSION ${CHORUSKIT_VERSION}
)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

configure_library(
    SHARED AUTOGEN
    VENDOR "Sine Striker"
)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem qBreakpad CkAppCore qastool::core
    LINKS_PRIVATE SingleApplication
    INCLUDE_PRIVATE .
)

include(config/qasc.cmake)