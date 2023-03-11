project(QsMedia
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_extra_includes)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT INCLUDE_SUBDIRS
    SOURCES ${_src}
    QT_LINKS Core
    QT_INCLUDES_PRIVATE Core
    LINKS QMCoreLib
    INCLUDES ${_extra_includes}
)