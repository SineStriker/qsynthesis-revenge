project(QMWidgets
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
    SOURCES ${_src}
    QT_LINKS Widgets Svg
    QT_INCLUDES_PRIVATE Core Gui Widgets
    LINKS QMCoreLib
    LINKS_PRIVATE
    INCLUDES ${_extra_includes}
    AUTO_INCLUDE_CURRENT
    AUTO_INCLUDE_DIRS Global Layout Metatypes Static Widgets ../..
    INCLUDE_SUBDIRS_PRIVATE
)