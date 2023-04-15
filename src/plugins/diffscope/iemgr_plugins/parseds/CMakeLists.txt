project(parseds VERSION 0.1.0.0)

set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "FlutyDeer"
    MACRO_PREFIX PARSEDS
    METADATA_IN plugin.json.in
    PLUGIN_NAME ParseDs
)

ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsSVS QsFramework ExtensionSystem DiffScope::iemgr
    LINKS_PRIVATE QMidi
    AUTO_INCLUDE_CURRENT
)

ck_add_translations(${_plugin}_translations
    PREFIX ParseDs
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

add_dependencies(${_plugin} ${_plugin}_translations_lrelease)

ck_add_attaches(${_plugin}
    # SRC Res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers(.)