project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    DIRECTORIES widgets private
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} res/res.qrc
    QT_LIBRARIES Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES res/NativeWindow.res.json
    ABSOLUTE_PATH ${SUB_RES_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES res/translations/*.qm
    ABSOLUTE_PATH ${SUB_RES_OUTPUT_DIR}/NativeWindow.res/translations
)
qs_add_res(${PROJECT_NAME}
    FILES res/themes/*.json
    ABSOLUTE_PATH ${SUB_RES_OUTPUT_DIR}/NativeWindow.res/themes
)