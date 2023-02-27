include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

set_property(GLOBAL PROPERTY CHORUSKIT_WINMAIN_SRC "${_src}")
