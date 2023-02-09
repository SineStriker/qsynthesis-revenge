project(UstPorter
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("IceKyrin")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    LINKS DsGui
)
