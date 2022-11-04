project(LabelVoice
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

# Set global variable for current project
set(INNER_PROJECT ${PROJECT_NAME})
set(INNER_VERSION ${PROJECT_VERSION})
set(INNER_PLUGIN_DIR ${APP_PLUGINS_DIR})

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Staging)
add_subdirectory(WinMain)

if (ENABLE_LV_PLUGINS)
    add_subdirectory(Plugin)
endif()
