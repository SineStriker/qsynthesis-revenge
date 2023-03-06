project(diffscope_coreplugin VERSION 0.1.0.0)

get_subdirs(_dirs)
set(_extra_includes)

add_files(_src
    CURRENT_RECURSE
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_plugin(${PROJECT_NAME}
    AUTHOR_NAME "Sine Striker"
    FILE_DESC "DiffScope Core Plugin"
    PRODUCT_NAME ${PROJECT_NAME}
    PARENT DiffScope
    SUBDIR plugins
    MACRO_PREFIX CORE
    OUTPUT_NAME Core
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS QsFramework ExtensionSystem
    INCLUDES ${_dirs} ${_extra_includes}
)

if(TRUE)
    set(PLUGIN_METADATA_VERSION
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}_${PROJECT_VERSION_TWEAK})
    set(PLUGIN_METADATA_COMPAT_VERSION 0.0.0_0)
    configure_file(
        plugin.json.in
        ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata/plugin.json
        @ONLY
    )
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/QtPluginMetadata)
endif()