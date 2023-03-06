include_guard(DIRECTORY)

#[[
    ck_add_plugin(
        AUTHOR_NAME      <var>
        FILE_DESC        <var>
        PRODUCT_NAME     <var>
        PARENT           <var>
        SUBDIR           <var>
        
        [OUTPUT_NAME     <var>]
        [MACRO_PREFIX    <var>]
        [COPYRIGHT_YEAR  <var>]
    )

    flags:
        AS_TEST:         ignore metadata and skip it when deploying

    usage:
        add a plugin with metadata
]] #
function(ck_add_plugin _target)
    set(options AS_TEST)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME MACRO_PREFIX PARENT SUBDIR OUTPUT_NAME COPYRIGHT_YEAR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    
    if(NOT FUNC_MACRO_PREFIX)
        string(TOUPPER ${_target} _prefix)
    else()
        set(_prefix ${FUNC_MACRO_PREFIX})
    endif()

    # Add library
    add_library(${_target} SHARED)

    target_compile_definitions(${_target} PRIVATE ${_prefix}_LIBRARY)

    if(FUNC_OUTPUT_NAME)
        set(_output_name ${FUNC_OUTPUT_NAME})
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_output_name})
    else()
        set(_output_name ${_target})
    endif()

    if(NOT FUNC_AS_TEST)
        if(NOT FUNC_PRODUCT_NAME)
            set(_product_name ${_target})
        else()
            set(_product_name ${FUNC_PRODUCT_NAME})
        endif()

        if(FUNC_COPYRIGHT_YEAR)
            set(_copyright_year ${FUNC_COPYRIGHT_YEAR})
        else()
            set(_copyright_year ${TIME_PROJECT_START_YEAR}-${TIME_CURRENT_YEAR})
        endif()

        # Add embedded resources
        if(WIN32)
            if(CONFIG_CMAKE_RANDOM_CONFIGURE_FILE)
                string(RANDOM LENGTH 8 _rand)
                set(rc_name ${CMAKE_CURRENT_BINARY_DIR}/res_${_rand}.rc)
            else()
                set(rc_name ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
            endif()

            # configure rc
            set(WIN32_EXPORT_NAME ${_output_name})
            set(WIN32_COPYRIGHT_YEAR "${_copyright_year}")
            set(WIN32_AUTHOR_NAME "${FUNC_AUTHOR_NAME}")
            set(WIN32_FILE_DESC "${FUNC_FILE_DESC}")
            set(WIN32_PRODUCT_NAME "${_product_name}")
            configure_file(
                ${WIN32_DLL_RC}
                ${rc_name}
                @ONLY
            )
            target_sources(${_target} PRIVATE ${rc_name})
        elseif(APPLE)
            # configure mac plist
            # set_target_properties(${_target} PROPERTIES
            # FRAMEWORK TRUE
            # FRAMEWORK_VERSION CXX
            # MACOSX_FRAMEWORK_NAME ${_output_name}
            # MACOSX_FRAMEWORK_IDENTIFIER ${_product_name}
            # MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
            # MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            # )
        endif()
    endif()

    set(_parent_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${FUNC_PARENT})

    if(FUNC_SUBDIR)
        set(_out_dir ${_parent_dir}/${FUNC_SUBDIR})
    else()
        set(_out_dir ${_parent_dir})
    endif()

    # Settle plugin
    set_target_properties(${_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
        LIBRARY_OUTPUT_DIRECTORY ${_out_dir}
        ARCHIVE_OUTPUT_DIRECTORY ${_out_dir}
    )

    # if(WIN32)
    # set_target_properties(${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${_out_dir})
    # set_target_properties(${_target}
    # PROPERTIES
    # LIBRARY_OUTPUT_DIRECTORY ${_parent_dir}
    # ARCHIVE_OUTPUT_DIRECTORY ${_parent_dir}
    # )
    # else()
    # set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${_out_dir})
    # endif()

    # ----------------- Template End -----------------
endfunction()
