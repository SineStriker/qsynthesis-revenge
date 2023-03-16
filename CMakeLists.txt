#[[

    If you use Qt Creator, open the CMakeLists.txt in src is suggested.

]]#

cmake_minimum_required(VERSION 3.17)

include(Config.cmake)

# ----------------------------------
# Main project
# ----------------------------------
project(${APP_TOOLSET_NAME} LANGUAGES C CXX VERSION ${APP_VERSION_VERBOSE})

set(NOT_QT_CREATOR on)

add_subdirectory(src)