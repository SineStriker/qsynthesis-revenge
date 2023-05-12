#[[

    If you use Qt Creator, open the CMakeLists.txt in src is suggested.

]]#

cmake_minimum_required(VERSION 3.17)

# ----------------------------------
# Main project
# ----------------------------------
project(ChorusKit LANGUAGES C CXX)

set(NOT_QT_CREATOR on)

add_subdirectory(src)