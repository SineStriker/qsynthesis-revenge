project(LabelVoice
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

set(INNER_PROJECT ${PROJECT_NAME})

set(INNER_VERSION ${PROJECT_VERSION})

set(INNER_PLUGIN_DIR ${APP_PLUGINS_DIR})

add_subdirectory(Core)

add_subdirectory(Staging)

if (ENABLE_LV_PLUGINS)
    add_subdirectory(Plugin)
endif()

add_subdirectory(WinMain)
