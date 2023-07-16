project(svgxicon
    VERSION 0.0.0.1
    LANGUAGES CXX
)

ck_add_files(_src
    CURRENT
    DIRS ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "Sine Striker")

ck_target_components(${_plugin}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Svg
    QT_INCLUDES_PRIVATE Core Widgets
    LINKS QMGui
)