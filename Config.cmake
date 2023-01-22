include_guard(GLOBAL)

# The macro works same as "option"
macro(option_ex _name)
    if(NOT DEFINED ${_name})
        set(${_name} ${ARGN})
    endif()
endmacro()

# The function checks if the current operating system is supported
function(check_os)
    set(_linux OFF)
    set(_name)

    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(_linux ON)
        set(_name Linux)
    elseif(WIN32)
        set(_name Windows)
    elseif(APPLE)
        set(_name Macintosh)
    else()
        message(FATAL_ERROR "Unsupported System !!!")
    endif()
    message(STATUS "[INFO] Current System is ${_name}")

    set(LINUX ${_linux} CACHE BOOL INTERNAL FORCE)
endfunction()

# Check environment
check_os()

# Project Configurations
option_ex(APP_TOOLSET_NAME ChorusKit)
option_ex(APP_TOOLSET_LIST DiffSinger)
option_ex(APP_VERSION_VERBOSE 0.0.1.7)
option_ex(APP_DEPLOY off)
option_ex(APP_LIB_DIR lib)
option_ex(APP_TOOLS_DIR bin)
option_ex(APP_PLUGINS_DIR plugins) # Do not change
option_ex(APP_LOCALES_DIR translations) # Do not change
option_ex(APP_RES_DIR resources)
option_ex(APP_PUBLIC_PLUGINS_DIR qsplugins)

option_ex(BUILD_TEST off)

option_ex(CONFIG_WIN32_DEBUG on)
