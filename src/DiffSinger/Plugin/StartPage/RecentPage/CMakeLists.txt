project(RecentPage
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    LINKS DsMedium
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "OpenVPI"
    WIN32_FILE_DESC "DiffSinger StartPage"
)
