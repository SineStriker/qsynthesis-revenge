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

    set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
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

    # ----------------- Template End -----------------

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)

    unset(_version)
    unset(_cxx_standard)
endmacro()
