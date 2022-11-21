project(LvEmptyTemplate
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS LvCore
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "LabelVoice Plugin for Generate Project"
)
