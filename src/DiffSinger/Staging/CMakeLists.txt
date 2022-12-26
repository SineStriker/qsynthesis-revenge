project(DsStaging LANGUAGES CXX VERSION ${SUB_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library(
    SOURCES ${_src} res/res.qrc
    ENABLE_SHARED
    QT_LIBRARIES Widgets Svg
    QT_PRIVATE_INCLUDES Gui Widgets
    LINKS DsMedium
    INCLUDES include src
    WIN32_FILE_DESC "DiffSinger Main Module"
    WIN32_PRODUCT_NAME "DiffSinger.Staging"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    APP_PLUGINS_DIR="${APP_PLUGINS_DIR}"
    APP_TOOLS_DIR="${APP_TOOLS_DIR}"
    APP_LOCALES_DIR="${APP_LOCALES_DIR}"
    APP_RES_DIR="${APP_RES_DIR}"
    APP_RES_BUILTIN_DIR="${APP_RES_BUILTIN_DIR}"
    APP_RES_PLUGINS_DIR="${APP_RES_PLUGINS_DIR}"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    APP_NAME="${SUB_PROJECT}"
    APP_VERSION="${SUB_VERSION_SHORT}"
    APP_DESCRIPTION="${SUB_DESCRIPTION}"
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)
