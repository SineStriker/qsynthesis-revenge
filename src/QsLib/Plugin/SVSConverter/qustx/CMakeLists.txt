project(qustx
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES Coders Ustx Util
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "QSynthesis Plugin for Project Convertion"
)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

settle_plugin(${PROJECT_NAME} svsconverters)
