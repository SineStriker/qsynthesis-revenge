project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES Widgets
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    LINKS QsFramework
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "QSynthesis Plugin for Window Decoration"
)

target_link_libraries(${PROJECT_NAME} PRIVATE FramelessHelper::FramelessHelperWidgets)

settle_plugin(${PROJECT_NAME} windowfactories)
