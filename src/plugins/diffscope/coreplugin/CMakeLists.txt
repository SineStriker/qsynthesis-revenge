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
    INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LINKS Core Widgets
    LINKS QsFramework ExtensionSystem CkAppCore
    INCLUDES ${_extra_includes}
)

ck_add_translations(${_plugin}_translations
    PREFIX Core
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR $<TARGET_FILE_DIR:${_plugin}>/share/Core.res/translations
    QM_TARGET ${_plugin}
)

ck_add_attaches(${_plugin}
    SRC Res/Core.res.json DEST $<TARGET_FILE_DIR:${_plugin}>/share
    SRC Res/themes/*.json DEST $<TARGET_FILE_DIR:${_plugin}>/share/Core.res/themes
)