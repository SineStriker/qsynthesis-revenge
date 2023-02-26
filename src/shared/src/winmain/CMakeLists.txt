include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

project(WinMain
    LANGUAGES CXX
    VERSION 0.0.1.1
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

set_property(GLOBAL PROPERTY WIN_MAIN_SRC "${_src}")
