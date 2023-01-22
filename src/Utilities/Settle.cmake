set(_tool_exes)

# Target Collector
get_all_targets(_all_targets)

foreach(_target ${_all_targets})
    get_target_property(_target_type ${_target} TC_TARGET_TYPE)

    if(${_target_type} STREQUAL EXECUTABLE)
        get_target_property(_exe_type ${_target} TC_EXECUTABLE_TYPE)

        if(${_exe_type} STREQUAL Tool)
            list(APPEND _tool_exes ${_target})
        endif()
    endif()
endforeach()

foreach(_exe ${_tool_exes})
    set_target_properties(
        ${_exe} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_TOOLS_DIR}
    )
endforeach()
