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

configure_plugin("Sine Striker" _plugin)

ck_target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LINKS Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)