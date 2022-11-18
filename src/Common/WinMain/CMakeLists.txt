project(WinMain
    LANGUAGES CXX
    VERSION 0.0.1.1
)

add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

set_property(GLOBAL PROPERTY WIN_MAIN_SRC "${_src}")