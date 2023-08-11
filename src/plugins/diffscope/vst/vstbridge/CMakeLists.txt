project(vstbridge VERSION 0.0.0.0)

ck_find_qt_module(RemoteObjects)

set(_library ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

qt5_generate_repc(_src ../VstBridge.rep REPLICA)

#ck_add_test(${_library})

ck_add_library(${_library} SHARED AUTOGEN
    NAME ${PROJECT_NAME}
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_APPLICATION} VST bridge."
)

ck_target_components(${_library}
    AUTO_INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets Network RemoteObjects
    LINKS QsFramework
    LINKS_PRIVATE vstglobal QSystemSemaphoreExtended
    INCLUDE_PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

ck_install_headers()