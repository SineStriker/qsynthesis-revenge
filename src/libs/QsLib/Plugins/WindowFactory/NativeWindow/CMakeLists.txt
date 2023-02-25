project(NativeWindow
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(_dirs Widgets private)
set(_res_dir Res)

add_files(_src
    CURRENT
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("Sine Striker" _plugin)

target_components(${_plugin}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LIBRARIES Widgets
    LINKS QsFramework
    LINKS_PRIVATE FramelessHelper::FramelessHelperWidgets
)

# target_create_translations(${_plugin} SOURCES ${_src}
#     LOCALES zh_CN zh_HK ja_JP
#     DESTINATION ${_res_dir}/translations
#     PREFIX ${PROJECT_NAME}
#     CREATE_ONCE
# )

# Copy resources
# qs_add_res(${_plugin}
#     FILES ${_res_dir}/${PROJECT_NAME}.res.json
#     RELATIVE_PATH .
# )
# qs_add_res(${_plugin}
#     FILES ${_res_dir}/translations/*.qm
#     RELATIVE_PATH ${PROJECT_NAME}.res/translations
# )
# qs_add_res(${_plugin}
#     FILES ${_res_dir}/themes/*.json
#     RELATIVE_PATH ${PROJECT_NAME}.res/themes
# )