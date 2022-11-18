# Only link public libraries
macro(configure_plugin)
    set(options INCLUDE_CURRENT)
    set(oneValueArgs WIN32_AUTHOR_NAME WIN32_FILE_DESC)
    set(multiValueArgs SOURCES QT_LIBRARIES QT_PRIVATE_INCLUDES LINKS INCLUDES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    set(_qt_libs)
    set(_qt_incs)
    set(_target ${PROJECT_NAME})

    # ----------------- Template Begin -----------------
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    add_qt_module(_qt_libs ${FUNC_QT_LIBRARIES})
    add_qt_private_inc(_qt_incs ${FUNC_QT_PRIVATE_INCLUDES})

    # Add library
    add_library(${_target} SHARED ${FUNC_SOURCES})

    target_link_libraries(${_target} PUBLIC ${_qt_libs})
    target_link_libraries(${_target} PUBLIC ${FUNC_LINKS})

    target_include_directories(${_target} PRIVATE ${_qt_incs})
    target_include_directories(${_target} PUBLIC ${FUNC_INCLUDES})

    if(FUNC_INCLUDE_CURRENT)
        target_include_directories(${_target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    # Add embedded resources
    if(WIN32)
        # configure rc
        set(WIN32_EXPORT_NAME ${_target})
        set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
        set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
        set(WIN32_AUTHOR_NAME "${FUNC_WIN32_AUTHOR_NAME}")
        set(WIN32_FILE_DESC "${FUNC_WIN32_FILE_DESC}")
        set(WIN32_PRODUCT_NAME ${_target})
        configure_file(
            ${WIN32_DLL_RC}
            ${CMAKE_CURRENT_BINARY_DIR}/res.rc
            @ONLY
        )
        target_sources(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
    endif()

    set(_out_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SUB_PLUGIN_DIR}/${CURRENT_PLUGIN_CATEGORY})

    # Settle plugin
    if(WIN32)
        set_target_properties(
            ${_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${_out_dir}
        )
    else()
        add_custom_command(
            TARGET ${_target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${_out_dir}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${_target}> ${_out_dir}
        )
    endif()

    unset(_out_dir)

    set_target_properties(
        ${_target}
        PROPERTIES
        TC_TARGET_TYPE PLUGIN
        TC_PLUGIN_TYPE QsLib
        TC_PLUGIN_CATEGORY ${CURRENT_PLUGIN_CATEGORY}
    )

    # ----------------- Template End -----------------

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)
    unset(_target)
endmacro()
