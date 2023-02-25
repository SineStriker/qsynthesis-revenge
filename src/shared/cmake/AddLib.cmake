include_guard(DIRECTORY)

#[[
    qs_add_library(
        <target>
        AUTHOR_NAME      <var>
        FILE_DESC        <var>
        PRODUCT_NAME     <var>
        LIBRARY_TYPE     <var>

        [OUTPUT_NAME     <var>]
        [MACRO_PREFIX    <var>]
        [COPYRIGHT_YEAR  <var>]
    )

    flags:
        ENABLE_SHARED:   build shared library
        AS_TEST:         ignore metadata and skip it when deploying

    usage:
        add a library with metadata
#]]
function(qs_add_library _target)
    set(options ENABLE_SHARED AS_TEST)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME LIBRARY_TYPE MACRO_PREFIX OUTPUT_NAME COPYRIGHT_YEAR)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT FUNC_MACRO_PREFIX)
        string(TOUPPER ${_target} _prefix)
    else()
        set(_prefix ${FUNC_MACRO_PREFIX})
    endif()

    # ----------------- Template Begin -----------------
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # Library type
    if(FUNC_ENABLE_SHARED)
        set(${_prefix}_BUILD_STATIC OFF)
    else()
        set(${_prefix}_BUILD_STATIC ON)
    endif()

    set(${_prefix}_BUILD_STATIC ${_prefix}_BUILD_STATIC PARENT_SCOPE)

    # Add library target
    if(${_prefix}_BUILD_STATIC)
        add_library(${_target} STATIC)
        target_compile_definitions(${_target} PUBLIC ${_prefix}_STATIC)
    else()
        add_library(${_target} SHARED)
    endif()

    target_compile_definitions(${_target} PRIVATE ${_prefix}_LIBRARY)

    if(FUNC_OUTPUT_NAME)
        set(_output_name ${FUNC_OUTPUT_NAME})
        set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_output_name})
    else()
        set(_output_name ${_target})
    endif()

    if(FUNC_ENABLE_SHARED AND NOT FUNC_AS_TEST)
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
            string(RANDOM LENGTH 8 _rand)
            set(rc_name ${CMAKE_CURRENT_BINARY_DIR}/res_${_rand}.rc)

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

    # ----------------- Template End -----------------
endfunction()
