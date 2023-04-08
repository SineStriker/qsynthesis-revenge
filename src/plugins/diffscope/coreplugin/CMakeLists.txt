project(coreplugin VERSION 0.1.0.0)

set(_extra_includes)
set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "Sine Striker"
    MACRO_PREFIX CORE
    METADATA_IN plugin.json.in
    PLUGIN_NAME Core
)

ck_target_components(${_plugin}
    SOURCES ${_src} ${_res_dir}/core_res.qrc
    QT_LINKS Core Widgets
    LINKS QsFramework ExtensionSystem CkCore
    INCLUDES ${_extra_includes}
    AUTO_INCLUDE_CURRENT
)

ck_add_translations(${_plugin}_translations
    PREFIX Core
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

add_dependencies(${_plugin} ${_plugin}_translations_lrelease)

ck_add_attaches(${_plugin}
    SRC Res/Core.res.json DEST share
    SRC Res/themes/*.json DEST share/Core.res/themes
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST share/Core.res
)

ck_install_headers(.)