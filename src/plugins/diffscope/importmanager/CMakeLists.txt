project(importmanager VERSION 0.1.0.0)

set(_extra_includes)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin_base(_plugin
    METADATA_IN plugin.json.in
    PLUGIN_NAME ImportManager
    MACRO_PREFIX IMPORTMANAGER
)

ck_target_components(${_plugin}
    INCLUDE_CURRENT INCLUDE_SUBDIRS
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework ExtensionSystem DiffScope::coreplugin
    INCLUDES ${_extra_includes}
)

ck_target_res(${_plugin}
    DESCRIPTION "DiffScope Import Manger Plugin"
    COPYRIGHT "FlutyDeer"
)