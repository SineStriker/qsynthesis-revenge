include_guard(DIRECTORY)

#[[
    qs_add_plugin(
        AUTHOR_NAME      <var>
        FILE_DESC        <var>
        PRODUCT_NAME     <var>
        PARENT           <var>
        SUBDIR           <var>
        CATEGORY         <var>
    )

    usage:
        add a plugin with metadata
]] #
function(qs_add_plugin _target)
    set(options)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME PARENT SUBDIR CATEGORY)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    if(NOT FUNC_PRODUCT_NAME)
        set(_product_name ${_target})
    else()
        set(_product_name ${FUNC_PRODUCT_NAME})
    endif()

    # ----------------- Template Begin -----------------
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # Add library
    add_library(${_target} SHARED)

    # Add embedded resources
    if(WIN32)
        # configure rc
        set(WIN32_EXPORT_NAME ${_target})
        set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
        set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
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
        set_target_properties(${_target} PROPERTIES
            FRAMEWORK TRUE
            FRAMEWORK_VERSION CXX
            MACOSX_FRAMEWORK_NAME ${_target}
            MACOSX_FRAMEWORK_IDENTIFIER ${_product_name}
            MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )
    endif()

    set(_out_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FUNC_SUBDIR}/${FUNC_CATEGORY})

    # Settle plugin
    # if(WIN32)
    set_target_properties(
        ${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
    )

    # else()
    # add_custom_command(
    # TARGET ${_target}
    # POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E make_directory ${_out_dir}
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${_target}> ${_out_dir}
    # )
    # endif()
    set_target_properties(
        ${_target}
        PROPERTIES
        TC_TARGET_TYPE PLUGIN
        TC_PLUGIN_PARENT ${FUNC_PARENT}
        TC_PLUGIN_SUBDIR ${FUNC_SUBDIR}
        TC_PLUGIN_CATEGORY ${FUNC_CATEGORY}
    )

    # ----------------- Template End -----------------
endfunction()
