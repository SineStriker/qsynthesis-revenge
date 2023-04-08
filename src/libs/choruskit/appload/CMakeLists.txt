project(CkAppLoad
    LANGUAGES CXX
    VERSION ${CHORUSKIT_VERSION}
)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem qBreakpad CkAppCore
    LINKS_PRIVATE SingleApplication qastool::core
    AUTO_INCLUDE_CURRENT
)

configure_ck_library(${PROJECT_NAME})

include(config/qasc.cmake)