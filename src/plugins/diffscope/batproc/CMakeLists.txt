project(batproc VERSION 0.1.0.0)

set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "CrSjimo"
    MACRO_PREFIX BATPROC_
    METADATA_IN plugin.json.in
    PLUGIN_NAME batproc
    # META_SUBDIRS plugins
)

ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src} ${_res_dir}/batproc_res.qrc
    QT_LINKS Core Widgets Network Qml
    LINKS QsSVS QsFramework ExtensionSystem DiffScope::coreplugin
    LINKS_PRIVATE QMidi QNrbfFormat
    AUTO_INCLUDE_CURRENT
)

ck_add_translations(${_plugin}_translations
    PREFIX BatProc
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

add_dependencies(${_plugin} ${_plugin}_translations_lrelease)

ck_add_attaches(${_plugin}
    SRC Res/batproc.res.json DEST share
    SRC Res/themes/*.json DEST share/batproc.res/themes
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST share/batproc.res
)

ck_install_headers(.)
