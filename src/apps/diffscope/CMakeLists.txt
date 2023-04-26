project(DiffScope
    LANGUAGES CXX
    VERSION ${CONFIG_APP_VERSION}
)

ck_get_short_version(PROJECT_VERSION_SHORT ${PROJECT_VERSION} 3)

ck_add_appmain(${PROJECT_NAME} CkAppLoad ${ROOT_APPMAIN_TYPE}
    ENV
    "CHORUSKIT_APP_NAME_HINT=${PROJECT_NAME}"
    "CHORUSKIT_APP_VERSION_HINT=${PROJECT_VERSION}"
)

ck_target_res(${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "ChorusKit ${PROJECT_NAME}"
    COPYRIGHT ${ROOT_OPENVPI_COPYRIGHT}
    ICO app.ico
    ICNS app.icns
    MANIFEST MAC_BUNDLE
)

# Set configration file variables
set(APP_CONFIG_CORE_NAME "Core")
set(APP_CONFIG_SPLASH_IMAGE "splash.png")
set(APP_CONFIG_PADDING 20)

# Configure configuration file
set(_config_file ${CMAKE_CURRENT_BINARY_DIR}/config.json)
configure_file(config.json.in ${_config_file} @ONLY)

ck_add_attaches(${PROJECT_NAME}
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST ${CHORUSKIT_SHARE_OUTPUT_DIR}/${PROJECT_NAME}
)

# Install docs and platform files
ck_get_subdirs(_docs_dirs DIRECTORY docs ABSOLUTE)
ck_get_subdirs(_platform_dirs DIRECTORY share/${CHORUSKIT_PLATFORM_SHORT} ABSOLUTE)

ck_add_attaches(${PROJECT_NAME} INSTALL_ONLY
    SRC ${_docs_dirs} DEST ${CHORUSKIT_DOCS_OUTPUT_DIR}
    SRC ${_platform_dirs} DEST ${CHORUSKIT_SHARE_OUTPUT_DIR}
)

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

]] #