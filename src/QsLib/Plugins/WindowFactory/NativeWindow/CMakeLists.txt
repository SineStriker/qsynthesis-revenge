project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(_dirs widgets private)
set(_res_dir Res)

add_files(_src
    CURRENT
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LIBRARIES Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION ${_res_dir}/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/NativeWindow.res.json
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/translations/*.qm
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/NativeWindow.res/translations
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/themes/*.json
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/NativeWindow.res/themes
)