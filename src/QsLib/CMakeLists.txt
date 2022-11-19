project(QsLib
    LANGUAGES CXX
    VERSION 0.2.18
)

include(Template.cmake)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(SUB_PLUGIN_DIR ${APP_RES_DIR}/${APP_RES_BUILTIN_DIR})

add_subdirectory(Utils)
add_subdirectory(SVS)

if (NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(Framework)
    add_subdirectory(Media)
    add_subdirectory(Integrate)
endif()

add_subdirectory(Plugin)
