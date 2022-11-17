# Copy dlls from vcpkg to build dir
if(TRUE)
    set(PREBUILT_DLL_LIST)

    if(APP_DEPLOY)
        set(_runtime_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_LIB_DIR})
    else()
        set(_runtime_output_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()

    if(WIN32)
        file(GLOB _dlls ${VCPKG_BINARY_DIR}/${VCPKG_BINARY_PAT})

        foreach(_dll ${_dlls})
            get_filename_component(_name ${_dll} NAME)
            set(_out_dll ${_runtime_output_dir}/${_name})

            add_custom_command(
                OUTPUT ${_out_dll}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_dll} ${_runtime_output_dir}
            )

            list(APPEND PREBUILT_DLL_LIST ${_out_dll})
        endforeach()
    endif()

    # Copy pre-built libraries
    add_custom_target(setup_deps DEPENDS ${PREBUILT_DLL_LIST})

    add_dependencies(${MAIN_TARGET} setup_deps)
endif()

# Collect translation targets
if(TRUE)
    set(_lupdate_targets)
    set(_lrelease_targets)

    foreach(_target ${CURRENT_ALL_TARGETS})
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

endif()