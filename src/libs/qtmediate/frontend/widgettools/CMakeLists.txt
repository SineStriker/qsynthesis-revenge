project(QMWidgetTools
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Widgets Svg
    QT_INCLUDES_PRIVATE Core Gui Widgets
    LINKS QMWidgets
    LINKS_PRIVATE
    AUTO_INCLUDE_CURRENT
    AUTO_INCLUDE_DIRS Global Dialogs Widgets Tools ../..
    INCLUDE_SUBDIRS_PRIVATE
)