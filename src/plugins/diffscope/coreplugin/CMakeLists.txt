project(coreplugin VERSION 0.1.0.0)

set(_res_dir Res)
set(_plugin ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_application_plugin(${_plugin}
    NAME Core
    COMPAT_VERSION 0.0.0.0
    VENDOR "Sine Striker"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
)

ck_target_components(${_plugin}
    SOURCES ${_src} ${_res_dir}/core_res.qrc
    QT_LINKS Core Widgets
    QT_INCLUDES_PRIVATE Core Gui Widgets
    LINKS QsFramework QsSVS acetree ExtensionSystem CkAppCore
    AUTO_INCLUDE_CURRENT
    INCLUDE_PRIVATE ${CK_HEADERS_OUTPUT_PATH}
)

ck_add_translations(${_plugin}_translations
    PREFIX Core
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

ck_add_attached_files(${_plugin}
    SRC Res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers()