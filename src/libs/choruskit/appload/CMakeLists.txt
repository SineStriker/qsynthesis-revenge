project(CkAppLoad
    LANGUAGES CXX
    VERSION ${CHORUSKIT_VERSION}
)

set(_extra_includes ${QASTOOL_INCLUDE_DIRS})

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem qBreakpad
    LINKS_PRIVATE SingleApplication
    INCLUDES ${_extra_includes}
)

configure_ck_library(${PROJECT_NAME})

include(config/qasc.cmake)