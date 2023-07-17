project(QMGui
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Gui Svg
    QT_INCLUDES_PRIVATE Core Gui
    LINKS QMCore
    LINKS_PRIVATE
    AUTO_INCLUDE_CURRENT
    AUTO_INCLUDE_DIRS Global Kernel Image Tools ../..
    INCLUDE_SUBDIRS_PRIVATE
)

ck_dir_skip_automoc(Image Tools)