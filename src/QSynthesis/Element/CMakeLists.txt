project(QUtaElem LANGUAGES CXX VERSION ${SUB_VERSION})

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
    LINKS QsBase QsIntegrate QUtaCore
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Elementary Module"
    WIN32_PRODUCT_NAME "QSynthesis.Elementary"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    APP_PLUGINS_DIR="${APP_PLUGINS_DIR}"
    APP_TOOLS_DIR="${APP_TOOLS_DIR}"
    APP_LOCALES_DIR="${APP_LOCALES_DIR}"
    APP_EXT_DIR="${APP_EXT_DIR}"
    APP_PUBLIC_PLUGINS_DIR="${APP_EXT_DIR}/${APP_PUBLIC_PLUGINS_DIR}"
    APP_PRIVATE_PLUGINS_DIR="${SUB_PLUGIN_DIR}"
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)
