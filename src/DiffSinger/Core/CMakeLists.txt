project(DsCore LANGUAGES CXX VERSION ${SUB_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

if(WIN32)
    list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.(h|cpp|hpp|cc)")
elseif(APPLE)
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.(h|cpp|hpp|cc)")
else()
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.(h|cpp|hpp|cc)")
endif()

configure_library(
    SOURCES ${_src} res/res.qrc
    ENABLE_SHARED
    QT_LIBRARIES Core
    LINKS QsSVS QsIntegrate
    INCLUDES include src
    WIN32_FILE_DESC "DiffSinger Core Module"
    WIN32_PRODUCT_NAME "DiffSinger.Core"
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
    APP_PLUGINS_DIR="${APP_PLUGINS_DIR}"
    APP_TOOLS_DIR="${APP_TOOLS_DIR}"
    APP_LOCALES_DIR="${APP_LOCALES_DIR}"
    APP_RES_DIR="${APP_RES_DIR}"
    APP_RES_BUILTIN_DIR="${APP_RES_BUILTIN_DIR}"
    APP_RES_PLUGINS_DIR="${APP_RES_PLUGINS_DIR}"
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)
