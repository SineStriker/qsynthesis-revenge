include_guard(DIRECTORY)

# Check if a variable is Defined, throw error if not
function(check_defined _var)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "${_var} not defined")
    endif()
endfunction()

# The macro works same as "option"
macro(option_ex _name)
    if(NOT DEFINED ${_name})
        set(${_name} ${ARGN})
    endif()
endmacro()

# The function checks if the current operating system is supported
function(check_pc_os)
    set(_linux OFF)
    set(_name)

    if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
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

    set(LINUX ${_linux} CACHE BOOL "If the current OS is Linux" FORCE)
endfunction()