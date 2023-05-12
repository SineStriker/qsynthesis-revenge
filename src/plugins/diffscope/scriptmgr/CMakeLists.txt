project(scriptmgr VERSION 0.1.0.0)

set(_res_dir Res)
set(_plugin ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_files(_script_src
    DIRS ${_res_dir}/scripts
    PATTERNS *.js
)

ck_add_application_plugin(${_plugin}
    NAME ScriptMgr
    COMPAT_VERSION 0.0.0.0
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    PLUGIN_JSON plugin.json.in
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
    TS_OPTIONS "-tr-function-alias" "qsTranslate=_tr"
)

ck_add_attached_files(${_plugin}
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers(.)
