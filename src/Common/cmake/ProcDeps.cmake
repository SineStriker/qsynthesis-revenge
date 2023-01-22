include_guard(DIRECTORY)

# Copy dlls from vcpkg to build dir
function(proc_deps _all_targets)
    set(_prebuilt_dlls)

    if(APP_DEPLOY OR NOT WIN32)
        set(_runtime_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_LIB_DIR})
    else()
        set(_runtime_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()

    # if(WIN32)
    file(GLOB _dlls ${VCPKG_BINARY_DIR}/${VCPKG_BINARY_PAT})

    foreach(_dll ${_dlls})
        get_filename_component(_name ${_dll} NAME)
        set(_out_dll ${_runtime_output_dir}/${_name})

        add_custom_command(
            OUTPUT ${_out_dll}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_dll} ${_runtime_output_dir}
        )

        list(APPEND _prebuilt_dlls ${_out_dll})
    endforeach()

    # endif()

    # Copy pre-built libraries
    add_custom_target(setup_deps DEPENDS ${_prebuilt_dlls})

    foreach(_target ${APP_TOOLSET_LIST})
        add_dependencies(${_target} setup_deps)
    endforeach()

    # Collect translation targets
    set(_lupdate_targets)
    set(_lrelease_targets)

    foreach(_target ${_all_targets})
        if(_target MATCHES ".+_lupdate")
            list(APPEND _lupdate_targets ${_target})
        endif()

        if(_target MATCHES ".+_lrelease")
            list(APPEND _lrelease_targets ${_target})
        endif()
    endforeach()

    add_custom_target(
        lupdate_all
        DEPENDS ${_lupdate_targets}
    )

    add_custom_target(
        lrelease_all
        DEPENDS ${_lrelease_targets}
    )

    add_custom_target(lrelease_clear)

    if(WIN32)
        add_custom_command(
            TARGET lrelease_clear
            COMMAND cmd /Q /C del /S /Q "*.qm"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    else()
        add_custom_command(
            TARGET lrelease_clear
            COMMAND bash -c "find . -name '*.qm' -type f -print -exec rm -rf {} \\;"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
endfunction()