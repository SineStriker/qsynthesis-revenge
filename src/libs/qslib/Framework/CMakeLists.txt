project(QsFramework
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_extra_includes)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp cc*.
)

configure_library()

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT INCLUDE_SUBDIRS
    SOURCES ${_src}
    QT_LINKS Widgets
    QT_INCLUDES_PRIVATE Core Widgets
    LINKS QMWidgets QsMedia QsSVS
    LINKS_PRIVATE
    INCLUDES ${_extra_includes}
)