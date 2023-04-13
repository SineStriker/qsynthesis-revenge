project(parsesvip3 VERSION 0.1.0.0)

set(_res_dir Res)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "FlutyDeer"
    MACRO_PREFIX PARSESVIP3
    METADATA_IN plugin.json.in
    PLUGIN_NAME ParseSvip3
)

ck_target_components(${_plugin} INCLUDE_CURRENT_PRIVATE INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsSVS QsFramework ExtensionSystem DiffScope::iemgr
    LINKS_PRIVATE QMidi
    AUTO_INCLUDE_CURRENT
)

ck_install_headers(.)