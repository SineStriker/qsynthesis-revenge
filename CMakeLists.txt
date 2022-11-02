# If you use Qt Creator, open the CMakeLists.txt in src is suggested.

cmake_minimum_required(VERSION 3.17)

# Same as option
macro(option_ex _name _val)
    if (NOT DEFINED ${_name})
        set(${_name} ${_val})
    endif()
endmacro()

# Pre-defined Variables
option_ex(MAIN_TARGET           LabelVoice)
option_ex(APP_VERSION_VERBOSE   0.0.1.3)
option_ex(APP_DEPLOY            OFF)
option_ex(APP_LIB_DIR           lib)
option_ex(APP_PLUGINS_DIR       plugins)

# Project
project(${MAIN_TARGET}
    LANGUAGES C CXX
    VERSION ${APP_VERSION_VERBOSE}
)

set(NOT_QT_CREATOR 1)

add_subdirectory(src)