project(UstPorter
    VERSION 0.0.0.1
    LANGUAGES CXX
)

set(_res_dir Res)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin("IceKyrin")

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LIBRARIES Widgets
    LINKS DsGui
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION ${_res_dir}/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/${PROJECT_NAME}.res.json
    RELATIVE_PATH .
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/translations/*.qm
    RELATIVE_PATH ${PROJECT_NAME}.res/translations
)
