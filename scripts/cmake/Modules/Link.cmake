function(delay_link_libraries _target _scope)
    foreach(_dll ${ARGN})
        get_target_property(_path ${_dll} IMPORTED_LOCATION_RELEASE)
        if (_path STREQUAL "_path-NOTFOUND")
            get_target_property(_output ${_dll} OUTPUT_NAME)
            if (_output STREQUAL "_output-NOTFOUND")
                if (MSVC)
                    set(_name "${_dll}.dll")
                else()
                    set(_name "lib${_dll}.dll")
                endif()
            else()
                set(_name "${_output}.dll")
            endif()
        else()
            get_filename_component(_name ${_path} NAME)
        endif()
        message("delay: ${_name}")
        target_link_options(${_target} ${_scope} "/DELAYLOAD:${_name}")
    endforeach()
endfunction()

function(deploy_shared_libraries)
    set(options PREBUILT RELATIVE CUT)
    set(oneValueArgs TARGET DESTINATION)
    set(multiValueArgs DEPENDENCIES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_target ${FUNC_TARGET})
    set(_dest ${FUNC_DESTINATION})

    foreach(_dll ${FUNC_DEPENDENCIES})
        if (FUNC_PREBUILT)
            if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
                get_target_property(_path ${_dll} IMPORTED_LOCATION_DEBUG)
                if (_path STREQUAL "_path-NOTFOUND")
                    get_target_property(_path ${_dll} IMPORTED_LOCATION_RELEASE)
                endif()
            else()
                get_target_property(_path ${_dll} IMPORTED_LOCATION_RELEASE)
            endif()
            if (_path STREQUAL "_path-NOTFOUND")
                message(FATAL_ERROR "Shared library ${_dll} not found.")
            endif()
            # get_filename_component(_name ${_path} NAME)
            # message("Add deploy task for \"${_name}\" to path \"${_dest}\"")
            if (FUNC_RELATIVE)
                add_custom_command(TARGET ${_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        $<TARGET_FILE_DIR:${_target}>/${_dest}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${_path} $<TARGET_FILE_DIR:${_target}>/${_dest}
                )
            else()
                add_custom_command(TARGET ${_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        ${_dest}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${_path} ${_dest}
                )
            endif()
            # Ignore cut
        else()
            # message("Add deploy task for ${_dll} to path \"${_dest}\"")
            if (FUNC_RELATIVE)
                add_custom_command(TARGET ${_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        $<TARGET_FILE_DIR:${_target}>/${_dest}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:${_dll}> $<TARGET_FILE_DIR:${_target}>/${_dest}
                )
            else()
                add_custom_command(TARGET ${_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        ${_dest}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:${_dll}> ${_dest}
                )
            endif()
            if (FUNC_CUT)
                add_custom_command(TARGET ${_target} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E rm
                        $<TARGET_FILE:${_dll}>
                )
            endif()
        endif()
    endforeach()
endfunction()

function(deploy_shared_libraries_self)
    set(options PREBUILT RELATIVE CUT)
    set(oneValueArgs TARGET DESTINATION)
    set(multiValueArgs DEPENDENCIES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_target ${FUNC_TARGET})
    set(_dest ${FUNC_DESTINATION})
    if (FUNC_RELATIVE)
        add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:${_target}>/${_dest}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${_target}> $<TARGET_FILE_DIR:${_target}>/${_dest}
        )
    else()
        add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
                ${_dest}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${_target}> ${_dest}
        )
    endif()
    if (FUNC_CUT)
        add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E rm
                $<TARGET_FILE:${_target}>
        )
    endif()
endfunction()
