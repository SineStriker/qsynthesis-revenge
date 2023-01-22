project(qmidi
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_subdirectory(QMidi)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsSVS
    LINKS_PRIVATE QMidi
)
