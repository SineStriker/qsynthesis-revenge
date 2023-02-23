include(${PROJECT_MODULES_DIR}/ListUtil.cmake)

project(WinMain
    LANGUAGES CXX
    VERSION 0.0.1.1
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

if(WIN32)
    list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.(h|cpp|hpp|cc)")
elseif(APPLE)
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.(h|cpp|hpp|cc)")
else()
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.(h|cpp|hpp|cc)")
endif()

set_property(GLOBAL PROPERTY WIN_MAIN_SRC "${_src}")
