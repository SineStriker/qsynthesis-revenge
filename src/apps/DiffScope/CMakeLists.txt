project(DiffScope
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(SUB_PLUGIN_DIR diffscope)

set(SUB_AUTHOR_NAME "OpenVPI Team")
set(SUB_FILE_DESC "DiffSinger Client")

# Add subdirectories
# add_subdirectory(Core)
# add_subdirectory(Gui)
# add_subdirectory(Host)
# add_subdirectory(Plugins)
add_subdirectory(App)
