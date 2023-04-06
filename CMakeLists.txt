#[[

    If you use Qt Creator, open the CMakeLists.txt in src is suggested.

]]#

cmake_minimum_required(VERSION 3.17)

include(Config.cmake)

# ----------------------------------
# Main project
# ----------------------------------
project(ChorusKit LANGUAGES C CXX VERSION ${CONFIG_APP_VERSION})

set(NOT_QT_CREATOR on)

add_subdirectory(src)