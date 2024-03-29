project(vstclient VERSION 0.1.0.0)

ck_find_qt_module(RemoteObjects)

set(_res_dir Res)
set(_plugin ${CURRENT_APPLICATION}__${PROJECT_NAME})

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

qt5_generate_repc(_src ../VstBridge.rep SOURCE)

ck_add_application_plugin(${_plugin}
    NAME VstClient
    COMPAT_VERSION 0.0.0.0
    VENDOR "CrSjimo"
    DESCRIPTION "${CURRENT_PLUGIN_DESC}"
    MACRO_PREFIX VST
)

ck_target_components(${_plugin}
    INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src} ${_res_dir}/vstclient_res.qrc
    QT_LINKS Core Widgets Network RemoteObjects
    LINKS QsSVS QsFramework ExtensionSystem DiffScope::coreplugin
    AUTO_INCLUDE_CURRENT
    INCLUDE_PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

ck_add_translations(${_plugin}_translations
    PREFIX Vst
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

ck_add_attached_files(${_plugin}
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers()
