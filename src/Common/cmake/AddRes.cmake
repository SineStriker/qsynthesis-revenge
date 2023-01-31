#[[
    qs_add_res(
        <target>
        FILES           <files>
        DIRS            <dirs>
        [RELATIVE_PATH  <rel_path>]
    )

    usage:
        copy files and dirs to destination relative to target after build
]] #
function(qs_add_res _target)
    set(options)
    set(oneValueArgs RELATIVE_PATH)
    set(multiValueArgs FILES DIRS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(FUNC_RELATIVE_PATH)
        set(_dest $<TARGET_FILE_DIR:${_target}>/${FUNC_RELATIVE_PATH})
    else()
        set(_dest $<TARGET_FILE_DIR:${_target}>)
    endif()

    # Make dir
    add_custom_command(
        TARGET ${_target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_dest}
    )

    # Copy files
    if(FUNC_FILES)
        file(GLOB _files ${FUNC_FILES})

        foreach(_file ${_files})
            add_custom_command(
                TARGET ${_target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${_dest}
            )
        endforeach()
    endif()

    # Copy dirs
    if(FUNC_DIRS)
        foreach(_file ${FUNC_DIRS})
            get_filename_component(_dir ${_file} DIRECTORY)
            get_filename_component(_name ${_file} NAME)
            add_custom_command(
                TARGET ${_target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${dir}/${_name} ${_dest}
            )
        endforeach()
    endif()
endfunction()
