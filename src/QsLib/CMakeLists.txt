cmake_minimum_required(VERSION 3.17)

add_subdirectory(Utils)
add_subdirectory(SVS)

if (NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(Framework)
    add_subdirectory(Media)
endif()
