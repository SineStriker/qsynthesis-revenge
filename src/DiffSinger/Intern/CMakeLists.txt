project(DsIntern LANGUAGES CXX VERSION ${SUB_VERSION})

set(_dirs Global Kernel Managers Windows)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffSinger.Intern"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} Res/res.qrc
    QT_LIBRARIES Widgets
    LINKS DsMedium
    INCLUDES ${_dirs}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    APP_NAME="${SUB_PROJECT}"
    APP_VERSION="${SUB_VERSION_SHORT}"
    APP_DESCRIPTION="${SUB_DESCRIPTION}"
    APP_PLUGINS_DIR="${APP_PLUGINS_DIR}"
    APP_LOCALES_DIR="${APP_LOCALES_DIR}"
    APP_TOOLS_DIR="${APP_TOOLS_DIR}"
    APP_RES_DIR="${APP_RES_DIR}"
    APP_PUBLIC_PLUGINS_DIR="${APP_RES_DIR}/${APP_PUBLIC_PLUGINS_DIR}"
    APP_PRIVATE_PLUGINS_DIR="${SUB_PLUGIN_DIR}"
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)
