project(DiffScope
    LANGUAGES CXX
    VERSION ${CHORUSKIT_VERSION}
)

ck_add_application(${PROJECT_NAME} CkAppLoad
    ENV "CHORUSKIT_APP_NAME_HINT=${PROJECT_NAME}" "CHORUSKIT_APP_VERSION_HINT=${PROJECT_VERSION}"
    ICO app.ico
    ICNS app.icns
    DESCRIPTION "ChorusKit ${PROJECT_NAME}"
)

# Set configration file variables
set(APP_CONFIG_CORE_NAME "Core")
set(APP_CONFIG_SPLASH_IMAGE "splash.png")
set(APP_CONFIG_PADDING 20)

# Configure configuration file
set(_config_file ${CMAKE_CURRENT_BINARY_DIR}/config.json)
configure_file(config.json.in ${_config_file} @ONLY)

# Copy files to application share dir
ck_add_application_files(${PROJECT_NAME}
    SRC ${_config_file} ${APP_CONFIG_SPLASH_IMAGE} DEST .
)

ck_add_application_files(${PROJECT_NAME}
    SKIP_BUILD BASE_SHARE_DIR
    SRC doc/* DEST doc/DiffScope
)

if(WIN32)
    # Add windows icons
    ck_add_application_files(${PROJECT_NAME}
        SKIP_BUILD
        SRC share/win-icons/* DEST icons
    )
elseif(APPLE)
else()
    # Add linux files
    ck_add_application_files(${PROJECT_NAME}
        SKIP_BUILD
        SRC share/applications share/icons DEST .
    )
endif()