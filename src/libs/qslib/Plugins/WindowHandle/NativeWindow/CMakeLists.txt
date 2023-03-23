project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(_dirs Widgets private)
set(_res_dir Res)

ck_add_files(_src
    CURRENT
    DIRS ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "Sine Striker")

ck_target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LINKS Widgets
    QT_INCLUDES_PRIVATE Core Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)