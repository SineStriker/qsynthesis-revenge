project(QSystemSemaphoreExtended VERSION 0.0.0.0)

set(_library ${PROJECT_NAME})

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

ck_add_library(${_library} SHARED AUTOGEN
    NAME ${PROJECT_NAME}
    VENDOR "CrSjimo"
    DESCRIPTION "QSystemSemaphore with advanced functions"
)

ck_target_components(${_library}
    AUTO_INCLUDE_CURRENT INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core
    QT_INCLUDES_PRIVATE Core
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Core_PRIVATE_INCLUDE_DIRS})

ck_install_headers()