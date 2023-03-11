project(coreplugin VERSION 0.1.0.0)

set(_extra_includes)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin_base(_plugin
    METADATA_IN plugin.json.in
    PLUGIN_NAME Core
    MACRO_PREFIX CORE
)

ck_target_components(${_plugin}
    INCLUDE_CURRENT INCLUDE_SUBDIRS
    SOURCES ${_src}
    QT_LINKS Core Widgets
    LINKS QsFramework ExtensionSystem
    INCLUDES ${_extra_includes}
)

ck_target_res(${_plugin}
    DESCRIPTION "DiffScope Core Plugin"
    COPYRIGHT "Sine Striker"
)