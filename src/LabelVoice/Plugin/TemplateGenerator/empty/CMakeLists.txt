project(EmptyTemplate
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    INCLUDE_CURRENT
    QT_LIBRARIES Core
    LINKS LvCore
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "LabelVoice Plugin for Initialize Project"
)

settle_plugin(${PROJECT_NAME} templategenerators)
