include_guard(DIRECTORY)
include(${CMAKE_CURRENT_LIST_DIR}/StringUtil.cmake)

# Check if a variable is Defined, throw error if not
function(check_defined _var)
    if(NOT DEFINED ${_var})
        message(FATAL_ERROR "${_var} not defined")
    endif()
endfunction()
