project(LabelVoice
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

include(Template.cmake)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_PLUGIN_DIR ${APP_RES_DIR}/${APP_RES_PLUGINS_DIR})

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Element)
add_subdirectory(Staging)
add_subdirectory(WinMain)

if (ENABLE_APP_PLUGINS)
    add_subdirectory(Plugin)
endif()
