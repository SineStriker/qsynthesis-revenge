#[[
    add_test_target(
        <target>

        SOURCES                 files...
        QT_LIBRARIES            qtlibs1...
        QT_PRIVATE_INCLUDES     qtlibs2...
        LINKS                   libs...
        INCLUDES                incs...

        [VERSION                <version>]
        [CXX_STANDARD           <standard>]

        [INCLUDE_CURRENT]
        [WIN32_EXE]
    )

    args:
        target:             target name
        files:              files to add to target
        qtlibs1:            Qt module names to link
        qtlibs2:            Qt module names to include private headers
        libs:               libraries to link publicly
        incs:               directories to include publicly
        version:            test target version
        standard:           cpp standard

    flags:
        INCLUDE_CURRENT:    include current directory publicly
        WIN32_EXE:          set target as Windows Application on Windows
]]#
macro(add_test_target _target)
    set(options WIN32_EXE INCLUDE_CURRENT)
    set(oneValueArgs VERSION CXX_STANDARD)
    set(multiValueArgs SOURCES QT_LIBRARIES QT_PRIVATE_INCLUDES LINKS INCLUDES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    set(_qt_libs)
    set(_qt_incs)
    set(_version 0.0.1.1)
    set(_cxx_standard 17)

    if(FUNC_VERSION)
        set(_version ${FUNC_VERSION})
    endif()

    if(FUNC_CXX_STANDARD)
        set(_cxx_standard ${FUNC_CXX_STANDARD})
    endif()

    # ----------------- Template Begin -----------------
    project(${_target} VERSION ${_version} LANGUAGES CXX)

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    set(CMAKE_CXX_STANDARD ${_cxx_standard})
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_qt_module(_qt_libs ${FUNC_QT_LIBRARIES})
    add_qt_private_inc(_qt_incs ${FUNC_QT_PRIVATE_INCLUDES})

    add_executable(${_target} ${FUNC_SOURCES})

    target_link_libraries(${_target} PRIVATE ${_qt_libs})
    target_link_libraries(${_target} PRIVATE ${FUNC_LINKS})

    target_include_directories(${_target} PRIVATE ${_qt_incs})
    target_include_directories(${_target} PRIVATE ${FUNC_INCLUDES})

    if(FUNC_INCLUDE_CURRENT)
        target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    if(FUNC_WIN32_EXE)
        set_target_properties(${_target} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()

    set_target_properties(${_target} PROPERTIES
        TC_TARGET_TYPE EXECUTABLE
        TC_EXECUTABLE_TYPE Test
    )

    # ----------------- Template End -----------------

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)

    unset(_version)
    unset(_cxx_standard)
endmacro()
