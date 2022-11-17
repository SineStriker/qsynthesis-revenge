project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES widgets private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src} res/res.qrc
    QT_LIBRARIES Widgets
    LINKS QsFramework
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "QSynthesis Plugin for Window Decoration"
)

target_link_libraries(${PROJECT_NAME} PRIVATE FramelessHelper::FramelessHelperWidgets)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
)
