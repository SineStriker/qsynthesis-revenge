cmake_minimum_required(VERSION 3.17)

include(Template.cmake)

set(INNER_PLUGIN_DIR ${APP_RES_DIR}/${APP_RES_BUILTIN_DIR})

add_subdirectory(Utils)
add_subdirectory(SVS)

if (NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(Framework)
    add_subdirectory(Media)
    add_subdirectory(Integrate)
endif()

add_subdirectory(Plugin)
