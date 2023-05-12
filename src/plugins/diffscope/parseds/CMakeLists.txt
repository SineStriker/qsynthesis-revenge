project(parseds VERSION 0.1.0.0)

set(_res_dir Res)
set(_plugin ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_application_plugin(${_plugin}
    NAME ParseDs
    COMPAT_VERSION 0.0.0.0
    VENDOR "FlutyDeer"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    PLUGIN_JSON plugin.json.in
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

ck_add_attached_files(${_plugin}
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers(.)