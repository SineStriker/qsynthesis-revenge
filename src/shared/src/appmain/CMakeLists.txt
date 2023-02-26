include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

set_property(GLOBAL PROPERTY WIN_MAIN_SRC "${_src}")
