set(PREBUILT_DLL_LIST)

if(WIN32)
    file(GLOB _dlls "${VCPKG_BINARY_DIR}/bin/*.dll")

    foreach(_dll ${_dlls})
        get_filename_component(_name ${_dll} NAME)
        set(_out_dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_name})

        add_custom_command(
            OUTPUT ${_out_dll}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_dll} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
        
        list(APPEND PREBUILT_DLL_LIST ${_out_dll})
    endforeach()
endif()

# Copy pre-built libraries
add_custom_target(setup_deps DEPENDS ${PREBUILT_DLL_LIST})

add_dependencies(${MAIN_TARGET} setup_deps)