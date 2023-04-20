project(scriptmgr VERSION 0.1.0.0)

set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_files(_script_src
    DIRS ${_res_dir}/scripts
    PATTERNS *.js
)

configure_plugin(_plugin "CrSjimo"
    MACRO_PREFIX SCRIPTMGR
    METADATA_IN plugin.json.in
    PLUGIN_NAME ScriptMgr
    # META_SUBDIRS plugins
)

ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src} ${_res_dir}/scriptmgr_res.qrc
    QT_LINKS Core Widgets Network Qml
    LINKS QsSVS QsFramework ExtensionSystem DiffScope::coreplugin
    LINKS_PRIVATE QMidi QNrbfFormat
    AUTO_INCLUDE_CURRENT
)

ck_add_translations(${_plugin}_translations
    PREFIX ScriptMgr
    TARGETS ${_plugin}
    SOURCES ${_script_src}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

add_dependencies(${_plugin} ${_plugin}_translations_lrelease)

ck_add_attaches(${_plugin}
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers(.)
