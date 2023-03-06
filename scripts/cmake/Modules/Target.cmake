include_guard(DIRECTORY)
include(${CMAKE_CURRENT_LIST_DIR}/FindUtil.cmake)

#[[
    target_components(
        <target>
        SOURCES                 files...
        QT_LIBRARIES            qtlibs1...
        QT_PRIVATE_INCLUDES     qtlibs2...
        LINKS                   libs1...
        LINKS_PRIVATE           libs2
        INCLUDES                incs1...
        INCLUDES_PRIVATE        incs2
    )

    args:
        target:             related target name
        files:              source file list
        qtlibs1:            Qt module names to link
        qtlibs2:            Qt module names to include private headers
        libs1:              libraries to link publicly
        libs2:              libraries to link privately
        incs1:              directories to include publicly
        incs2:              directories to include privately

    flags:
        INCLUDE_CURRENT:    include current directory publicly

    usage:
        add extra components to a target
#]]
function(target_components _target)
    set(options INCLUDE_CURRENT)
    set(oneValueArgs)
    set(multiValueArgs SOURCES QT_LIBRARIES QT_PRIVATE_INCLUDES LINKS LINKS_PRIVATE INCLUDES INCLUDES_PRIVATE)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    set(_qt_libs)
    set(_qt_incs)

    # ----------------- Template Begin -----------------
    target_sources(${_target} PRIVATE ${FUNC_SOURCES})

    add_qt_module(_qt_libs ${FUNC_QT_LIBRARIES})
    target_link_libraries(${_target} PUBLIC ${_qt_libs})
    target_link_libraries(${_target} PUBLIC ${FUNC_LINKS})
    target_link_libraries(${_target} PRIVATE ${FUNC_LINKS_PRIVATE})

    add_qt_private_inc(_qt_incs ${FUNC_QT_PRIVATE_INCLUDES})
    target_include_directories(${_target} PRIVATE ${_qt_incs})
    target_include_directories(${_target} PUBLIC ${FUNC_INCLUDES})
    target_include_directories(${_target} PRIVATE ${FUNC_INCLUDES_PRIVATE})

    if(FUNC_INCLUDE_CURRENT)
        target_include_directories(${_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    # ----------------- Template End -----------------

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)
endfunction()

#[[
    target_create_translations  <target> LOCALES langs...
                                [SOURCES files...]
                                [DESTINATION <dir>]
                                [QM_DIR <qmdir>]
                                [PREFIX <prefix>]
                                [CREATE_ONCE]

    args:
        target:      related target name
        files:       source file list, default to all .cpp/.cc files by querying SOURCES property at this point
        langs:       language names, e.g. zh_CN en_US
        dir:         output dir of .ts and. qm files, default to CMAKE_CURRENT_SOURCE_DIR
        qmdir:       generate qm files into <qmdir> instead of <dir> if specified, can be a generated expression
        prefix:      translation file prefix, default to target name

    flags:
        CREATE_ONCE: generate .ts files if not exists at configure
        POST_BUILD:  generate .qm files after build

    usage:
        add lupdate and lrelease target for a target using Qt linguist framework
#]]
function(target_create_translations _target)
    set(options CREATE_ONCE POST_BUILD)
    set(oneValueArgs DESTINATION PREFIX QM_DIR)
    set(multiValueArgs SOURCES LOCALES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_ts_files)

    if(FUNC_SOURCES)
        set(_src_files ${FUNC_SOURCES})
    else()
        get_target_property(_src_files ${_target} SOURCES)
        list(FILTER _src_files INCLUDE REGEX ".+\\.(cpp|cc)")
        list(FILTER _src_files EXCLUDE REGEX "(qasc|moc)_.+")
    endif()

    if(FUNC_DESTINATION)
        file(MAKE_DIRECTORY ${FUNC_DESTINATION})
        set(_ts_dest ${FUNC_DESTINATION})
    else()
        set(_ts_dest ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    set(_qm_dest ${_ts_dest})

    if(FUNC_QM_DIR)
        set(_qm_dest ${FUNC_QM_DIR})
    endif()

    if(FUNC_PREFIX)
        set(_prefix ${FUNC_PREFIX})
    else()
        set(_prefix ${_target})
    endif()

    foreach(_loc ${FUNC_LOCALES})
        list(APPEND _ts_files ${_ts_dest}/${_prefix}_${_loc}.ts)
    endforeach()

    find_qt_module(LinguistTools)

    if(FUNC_CREATE_ONCE)
        set(_create_once CREATE_ONCE)
    else()
        set(_create_once)
    endif()

    if(FUNC_POST_BUILD)
        set(_post_build POST_BUILD)
    else()
        set(_post_build)
    endif()

    add_lupdate_target(${_target}_lupdate
        INPUT ${_src_files}
        OUTPUT ${_ts_files}
        ${_create_once}
    )

    add_lrelease_target(${_target}_lrelease
        INPUT ${_ts_files}
        DESTINATION ${_qm_dest}
        DEPENDS ${_target}_lupdate
        RELATED_TARGET ${_target}
        ${_post_build}
    )
endfunction()

# Input: cxx source files
# Output: target ts files
function(add_lupdate_target _target)
    set(options CREATE_ONCE)
    set(oneValueArgs)
    set(multiValueArgs INPUT OUTPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LUPDATE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lupdate_deps ${_LUPDATE_DEPENDS})

    set(_my_sources ${_LUPDATE_INPUT})
    set(_my_tsfiles ${_LUPDATE_OUTPUT})

    add_custom_target(${_target} DEPENDS ${_lupdate_deps})

    get_target_property(_lupdate_exe ${Qt5_LUPDATE_EXECUTABLE} IMPORTED_LOCATION)

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

        get_filename_component(_ts_abs ${_ts_file} ABSOLUTE)

        if(_LUPDATE_CREATE_ONCE AND NOT EXISTS ${_ts_abs})
            message(STATUS "[INFO] Linguist update, generate ${_ts_name}")
            execute_process(
                COMMAND ${_lupdate_exe} ${_LUPDATE_OPTIONS} "@${_ts_lst_file}" -ts ${_ts_file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE _null
            )
        endif()

        add_custom_command(
            TARGET ${_target}
            COMMAND ${_lupdate_exe}
            ARGS ${_LUPDATE_OPTIONS} "@${_ts_lst_file}" -ts ${_ts_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_my_sources}
            BYPRODUCTS ${_ts_lst_file} VERBATIM
        )
    endforeach()
endfunction()

# Input: ts files
# Output: list to append qm files
function(add_lrelease_target _target)
    set(options CREATE_ONCE POST_BUILD)
    set(oneValueArgs DESTINATION OUTPUT RELATED_TARGET)
    set(multiValueArgs INPUT OPTIONS DEPENDS)

    cmake_parse_arguments(_LRELEASE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_lrelease_files ${_LRELEASE_INPUT})
    set(_lrelease_deps ${_LRELEASE_DEPENDS})

    add_custom_target(${_target} DEPENDS ${_lrelease_deps})

    get_target_property(_lrelease_exe ${Qt5_LRELEASE_EXECUTABLE} IMPORTED_LOCATION)

    set(_qm_files)

    foreach(_file ${_lrelease_files})
        get_filename_component(_abs_FILE ${_file} ABSOLUTE)
        get_filename_component(_qm_file ${_file} NAME)

        # everything before the last dot has to be considered the file name (including other dots)
        string(REGEX REPLACE "\\.[^.]*$" "" FILE_NAME ${_qm_file})
        get_source_file_property(output_location ${_abs_FILE} OUTPUT_LOCATION)

        if(output_location)
            file(MAKE_DIRECTORY "${output_location}")
            set(_qm_file "${output_location}/${FILE_NAME}.qm")
        elseif(_LRELEASE_DESTINATION)
            set(_qm_file "${_LRELEASE_DESTINATION}/${FILE_NAME}.qm")
        else()
            set(_qm_file "${CMAKE_CURRENT_BINARY_DIR}/${FILE_NAME}.qm")
        endif()

        get_filename_component(_qm_abs ${_qm_file} ABSOLUTE)
        get_filename_component(_qm_name ${_qm_file} NAME)

        if(_LRELEASE_CREATE_ONCE AND NOT EXISTS ${_qm_abs})
            message(STATUS "[INFO] Linguist Release, generate ${_qm_name}")
            execute_process(
                COMMAND ${_lrelease_exe} ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${_qm_file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE _null
            )
        endif()

        add_custom_command(
            TARGET ${_target}
            COMMAND ${_lrelease_exe}
            ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${_qm_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS ${_lrelease_files}
            VERBATIM
        )

        if(_LRELEASE_RELATED_TARGET AND _LRELEASE_POST_BUILD)
            add_custom_command(
                TARGET ${_LRELEASE_RELATED_TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${_LRELEASE_DESTINATION}
                COMMAND ${_lrelease_exe}
                ARGS ${_LRELEASE_OPTIONS} ${_abs_FILE} -qm ${_qm_file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                DEPENDS ${_lrelease_files}
                VERBATIM
            )
        endif()

        list(APPEND _qm_files ${_qm_file})
    endforeach()

    if(_LRELEASE_OUTPUT)
        set(${_LRELEASE_OUTPUT} ${_qm_files} PARENT_SCOPE)
    endif()
endfunction()
