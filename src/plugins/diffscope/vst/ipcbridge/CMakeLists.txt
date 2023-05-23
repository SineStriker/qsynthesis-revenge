project(ipcbridge VERSION 0.0.0.0)

set(_library ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

ck_add_library(${_library} AUTOGEN
    NAME ${PROJECT_NAME}
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_APPLICATION} VST server."
)

ck_target_components(${_library}
    AUTO_INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework
)

ck_install_headers()