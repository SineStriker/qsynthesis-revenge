# Only link public libraries
macro(configure_qs_lib)
    set(options INCLUDE_CURRENT ENABLE_SHARED)
    set(oneValueArgs WIN32_FILE_DESC WIN32_PRODUCT_NAME)
    set(multiValueArgs SOURCES QT_LIBRARIES QT_PRIVATE_INCLUDES LINKS INCLUDES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    set(_qt_libs)
    set(_qt_incs)
    set(_target ${PROJECT_NAME})
    string(TOUPPER ${PROJECT_NAME} _prefix)

    # ----------------- Template Begin -----------------

    # Library type
    if(${FUNC_ENABLE_SHARED})
        set(${_prefix}_BUILD_STATIC OFF)
    else()
        set(${_prefix}_BUILD_STATIC ON)
    endif()

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # Add library
    if(${_prefix}_BUILD_STATIC)
        add_library(${_target} STATIC ${FUNC_SOURCES})
        target_compile_definitions(${_target} PUBLIC ${_prefix}_STATIC)
    else()
        add_library(${_target} SHARED ${FUNC_SOURCES})
    endif()

    target_compile_definitions(${_target} PRIVATE ${_prefix}_LIBRARY)

    add_qt_module(_qt_libs ${FUNC_QT_LIBRARIES})
    target_link_libraries(${_target} PUBLIC ${_qt_libs})
    target_link_libraries(${_target} PUBLIC ${FUNC_LINKS})

    add_qt_private_inc(_qt_incs ${FUNC_QT_PRIVATE_INCLUDES})
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
        set(WIN32_AUTHOR_NAME "QSynthesis Team")
        set(WIN32_FILE_DESC "${FUNC_WIN32_FILE_DESC}")
        set(WIN32_PRODUCT_NAME "${FUNC_WIN32_PRODUCT_NAME}")
        configure_file(
            ${WIN32_DLL_RC}
            ${CMAKE_CURRENT_BINARY_DIR}/res.rc
            @ONLY
        )
        target_sources(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
    endif()

    # ----------------- Template End -----------------

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)
    unset(_target)
    unset(_prefix)
endmacro()
