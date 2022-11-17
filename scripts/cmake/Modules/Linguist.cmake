# In this project, we use our custom translation file management methods instead of official ones

include(${CMAKE_CURRENT_LIST_DIR}/FindUtil.cmake)

# Input: cxx source files
# Output: target ts files
function(add_lupdate_target _target)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs INPUT OUTPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LUPDATE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lupdate_options ${_LUPDATE_OPTIONS})
    set(_lupdate_deps ${_LUPDATE_DEPENDS})

    set(_my_sources ${_LUPDATE_INPUT})
    set(_my_tsfiles ${_LUPDATE_OUTPUT})

    add_custom_target(${_target} DEPENDS ${_lupdate_deps})

    foreach(_ts_file ${_my_tsfiles})
        # make a list file to call lupdate on, so we don't make our commands too
        # long for some systems
        get_filename_component(_ts_name ${_ts_file} NAME)
        set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ts_name}_lst_file")
        set(_lst_file_srcs)

        foreach(_lst_file_src ${_my_sources})
            set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
        endforeach()

        get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)

        foreach(_pro_include ${_inc_DIRS})
            get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
            set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
        endforeach()

        file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")

        add_custom_command(
            TARGET ${_target}
            COMMAND ${Qt5_LUPDATE_EXECUTABLE}
            ARGS ${_lupdate_options} "@${_ts_lst_file}" -ts ${_ts_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_my_sources}
            BYPRODUCTS ${_ts_lst_file} VERBATIM
        )
    endforeach()
endfunction()

# Input: ts files
# Output: list to append qm files
function(add_lrelease_target _target)
    set(options)
    set(oneValueArgs DESTINATION OUTPUT)
    set(multiValueArgs INPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LRELEASE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lrelease_files ${_LRELEASE_INPUT})
    set(_lrelease_deps ${_LRELEASE_DEPENDS})

    add_custom_target(${_target} DEPENDS ${_lrelease_deps})

    foreach(_file ${_lrelease_files})
        get_filename_component(_abs_FILE ${_file} ABSOLUTE)
        get_filename_component(qm ${_file} NAME)

        # everything before the last dot has to be considered the file name (including other dots)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${qm})
        get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)

        if(output_location)
            file(MAKE_DIRECTORY "${output_location}")
            set(qm "${output_location}/${FILE_NAME}.qm")
        elseif(_LRELEASE_DESTINATION)
            set(qm "${_LRELEASE_DESTINATION}/${FILE_NAME}.qm")
        else()
            set(qm "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.qm")
        endif()

        add_custom_command(
            TARGET ${_target}
            COMMAND ${Qt5_LRELEASE_EXECUTABLE}
            ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${qm}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_lrelease_files}
            VERBATIM
        )
        list(APPEND ${_qm_files} ${qm})
    endforeach()

    if(_LRELEASE_OUTPUT)
        set(${_LRELEASE_OUTPUT} ${_qm_files} PARENT_SCOPE)
    endif()
endfunction()

function(target_create_translations _target)
    set(options)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs SOURCES LOCALES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_ts_files)

    if(FUNC_DESTINATION)
        file(MAKE_DIRECTORY ${FUNC_DESTINATION})
        set(_ts_dest ${FUNC_DESTINATION})
    else()
        set(_ts_dest ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    foreach(_loc ${FUNC_LOCALES})
        list(APPEND _ts_files ${_ts_dest}/${_target}_${_loc}.ts)
    endforeach()

    find_qt_module(LinguistTools)

    add_lupdate_target(${_target}_lupdate
        INPUT ${FUNC_SOURCES}
        OUTPUT ${_ts_files}
    )

    add_lrelease_target(${_target}_lrelease
        INPUT ${_ts_files}
        DESTINATION ${_ts_dest}
        DEPENDS ${_target}_lupdate
    )
endfunction()
