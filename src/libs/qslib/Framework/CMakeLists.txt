project(QsFramework
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp cc*.
)

configure_library()

ck_target_components(${PROJECT_NAME}
    AUTO_INCLUDE_CURRENT INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Widgets
    QT_INCLUDES_PRIVATE Core Widgets
    LINKS QMWidgets QsMedia QsSVS
    LINKS_PRIVATE
    AUTO_INCLUDE_DIRS Global
    INCLUDE_PRIVATE ${CK_HEADERS_OUTPUT_PATH}
)