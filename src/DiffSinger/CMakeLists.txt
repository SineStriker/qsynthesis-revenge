project(DiffSinger
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

include(Template.cmake)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(SUB_DESCRIPTION "DiffSinger editor by OpenVPI.")
set(SUB_IDENTIFIER "com.OpenVPI.DiffSinger")

set(SUB_PLUGIN_DIR ${APP_RES_DIR}/${APP_RES_PLUGINS_DIR})

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Medium)
add_subdirectory(Staging)

if (ENABLE_APP_PLUGINS)
    add_subdirectory(Plugin)
endif()

add_subdirectory(WinMain)
