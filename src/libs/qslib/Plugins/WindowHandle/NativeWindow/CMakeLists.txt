project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(_dirs Widgets private)
set(_res_dir Res)

add_files(_src
    CURRENT
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker" _plugin)

target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LIBRARIES Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)