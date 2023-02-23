include_guard(DIRECTORY)

#[[
    qs_add_plugin(
        AUTHOR_NAME      <var>
        FILE_DESC        <var>
        PRODUCT_NAME     <var>
        PARENT           <var>
        SUBDIR           <var>
        
        [OUTPUT_NAME     <var>]
        [COPYRIGHT_YEAR  <var>]
    )

    usage:
        add a plugin with metadata
]] #
function(qs_add_plugin _target)
    set(options)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME PARENT SUBDIR OUTPUT_NAME COPYRIGHT_YEAR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # ----------------- Template Begin -----------------
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # Add library
    add_library(${_target} SHARED)

    if(NOT FUNC_PRODUCT_NAME)
        set(_product_name ${_target})
    else()
        set(_product_name ${FUNC_PRODUCT_NAME})
    endif()

    if(FUNC_OUTPUT_NAME)
        set(_output_name ${FUNC_OUTPUT_NAME})
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_output_name})
    else()
        set(_output_name ${_target})
    endif()

    if(FUNC_COPYRIGHT_YEAR)
        set(_copyright_year ${FUNC_COPYRIGHT_YEAR})
    else()
        set(_copyright_year ${TIME_PROJECT_START_YEAR}-${TIME_CURRENT_YEAR})
    endif()

    # Add embedded resources
    if(WIN32)
        # configure rc
        set(WIN32_EXPORT_NAME ${_output_name})
        set(WIN32_COPYRIGHT_YEAR "${_copyright_year}")
        set(WIN32_AUTHOR_NAME "${FUNC_AUTHOR_NAME}")
        set(WIN32_FILE_DESC "${FUNC_FILE_DESC}")
        set(WIN32_PRODUCT_NAME "${_product_name}")
        configure_file(
            ${WIN32_DLL_RC}
            ${CMAKE_CURRENT_BINARY_DIR}/res.rc
            @ONLY
        )
        target_sources(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
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

    set(_out_dir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${FUNC_SUBDIR})

    # Settle plugin
    if(WIN32)
        set_target_properties(
            ${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
        )
    else()
        set_target_properties(
            ${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${_out_dir}
        )
    endif()

    # ----------------- Template End -----------------
endfunction()
