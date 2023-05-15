# small letter plugin name; version
project(templateplg VERSION 0.0.0.0)

set(_res_dir Res)
set(_plugin ${CURRENT_APPLICATION}__${PROJECT_NAME})


ck_add_files(_src
        ALLDIRS
        PATTERNS *.h *.hpp *.cpp *.cc
        )

# Add additional resources
ck_add_files(_config_src
        DIRS ${_res_dir}/config
        PATTERNS *.js
        )

# NAME: plugin name; COMPAT_VERSION: version; VENDOR: author
ck_add_application_plugin(${_plugin}
        NAME TemplatePlg
        COMPAT_VERSION 0.0.0.0
        VENDOR "Wolf"
        DESCRIPTION "${CURRENT_PLUGIN_DESC}"
        )

# QT_LINKS: QT_library required; LINKS: other libraries;
# SOURCES: qrc sources;
# ExtensionSystem: the parent class of the plugin; coreplugin: the core plugin
ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
        SOURCES ${_src} ${_res_dir}/template_res.qrc
        QT_LINKS Core Widgets Gui Network
        LINKS QsSVS QsFramework ExtensionSystem DiffScope::coreplugin
        AUTO_INCLUDE_CURRENT
        )

# PREFIX trans_prefix
ck_add_translations(${_plugin}_translations
        PREFIX TemplatePlg
        TARGETS ${_plugin}
        SOURCES ${_config_src}
        LOCALES zh_CN zh_HK ja_JP
        TS_DIR Res/translations
        QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
        )

ck_add_attached_files(${_plugin}
        SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
        )

ck_install_headers(.)