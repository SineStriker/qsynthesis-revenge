project(DiffScope
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

ck_get_short_version(PROJECT_VERSION_SHORT ${PROJECT_VERSION} 3)

ck_add_appmain(${PROJECT_NAME} CkAppLoad ${ROOT_APPMAIN_TYPE}
    ENV
    "APP_NAME_HINT=${PROJECT_NAME}"
    "APP_VERSION_HINT=${PROJECT_VERSION_SHORT}"
)

ck_target_res(${PROJECT_NAME}
    DESCRIPTION "DiffSinger Client"
    COPYRIGHT ${ROOT_OPENVPI_COPYRIGHT}
    ICO win32/app.ico
    ICNS mac/app.icns
    MANIFEST
)

# Set configration file variables
set(APP_CONFIG_CORE_NAME "Core")
set(APP_CONFIG_SPLASH_IMAGE "splash.png")
set(APP_CONFIG_PADDING 20)

# Configure configuration file
set(_config_file ${CMAKE_CURRENT_BINARY_DIR}/config.json)
configure_file(config.json.in ${_config_file} @ONLY)

ck_add_attaches(${PROJECT_NAME}
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST ${ROOT_SHARE_OUTPUT_DIRECTORY}/${PROJECT_NAME}
)

if (CONFIG_CREATE_WIN_SHORTCUT)
    ck_add_win_shortcut(${PROJECT_NAME} ${CONFIG_OUTPUT_DIRECTORY} OUTPUT_NAME "ChorusKit ${PROJECT_NAME}")
endif()

#[[

    "copyright": {
        "pos": [-@APP_CONFIG_PADDING@, -@APP_CONFIG_PADDING@],
        "anchor": [-1, -1],
        "text": "@ROOT_OPENVPI_COPYRIGHT@",
        "fontColor": "#FFFFFF"
    },
    "title": {
        "pos": [@APP_CONFIG_PADDING@, @APP_CONFIG_PADDING@],
        "text": "CHORUS KIT",
        "fontColor": "#FFFFFF"
    },

]]#