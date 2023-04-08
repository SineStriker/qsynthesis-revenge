project(importmanager VERSION 0.1.0.0)

set(_extra_includes ${CURRENT_SUB_PROJECT_MAIN_DIR})

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "FlutyDeer"
    MACRO_PREFIX IMPORTMANAGER
    METADATA_IN plugin.json.in
    PLUGIN_NAME ImportManager
)

ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem DiffScope::coreplugin
    INCLUDES ${_extra_includes}
)

ck_install_headers(.)