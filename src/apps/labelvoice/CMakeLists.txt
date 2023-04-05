project(LabelVoice
    LANGUAGES CXX
    VERSION ${CONFIG_APP_VERSION}
)

include(Template.cmake)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(SUB_DESCRIPTION "Vocal synthesis annotation tool.")
set(SUB_IDENTIFIER "com.OpenVPI.LabelVoice")

set(SUB_PLUGIN_DIR ${APP_EXT_DIR}/labelvoice)

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Element)
add_subdirectory(Staging)
add_subdirectory(Plugin)
add_subdirectory(WinMain)
