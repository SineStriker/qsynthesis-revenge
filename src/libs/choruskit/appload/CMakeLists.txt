project(CKAppLoad
    LANGUAGES CXX
    VERSION 0.0.0.1
)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp *.qrc)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem qBreakpad
    LINKS_PRIVATE SingleApplication
)

ck_target_res(${PROJECT_NAME}
    DESCRIPTION "ChorusKit Application Loader"
    COPYRIGHT ${ROOT_OPENVPI_COPYRIGHT}
)