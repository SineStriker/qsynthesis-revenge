include(${PROJECT_MODULES_DIR}/Find.cmake)

macro(add_files _src)
    set(options CLEAR CURRENT)
    set(oneValueArgs)
    set(multiValueArgs DIRECTORIES)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_temp_src)

    if (FUNC_CLEAR)
        set(${_src})
    endif ()
    if (FUNC_CURRENT)
        file(GLOB _temp_src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
    endif ()
    foreach (_dir ${FUNC_DIRECTORIES})
        set(_temp)
        file(GLOB_RECURSE _temp ${_dir}/*.h ${_dir}/*.cpp)
        list(APPEND _temp_src ${_temp})
        unset(_temp)
    endforeach ()
    list(APPEND ${_src} ${_temp_src})

    unset(_temp_src)
endmacro()

macro(add_test_target _target)
    set(options WIN32_EXE INCLUDE_CURRENT)
    set(oneValueArgs VERSION CXX_STANDARD)
    set(multiValueArgs SOURCES QT_LIBRARIES QT_PRIVATE_INCLUDES LIBRARIES INCLUDE_DIRS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Parse vars
    set(_qt_libs)
    set(_qt_incs)
    set(_version 0.0.1.1)
    set(_cxx_standard 17)

    foreach (_lib ${FUNC_QT_LIBRARIES})
        add_qt_module(_qt_libs ${_lib})
    endforeach ()

    foreach (_inc ${FUNC_QT_PRIVATE_INCLUDES})
        add_qt_private_inc(_qt_incs ${_inc})
    endforeach ()

    if (FUNC_VERSION)
        set(_version ${FUNC_VERSION})
    endif ()

    if (FUNC_CXX_STANDARD)
        set(_cxx_standard ${FUNC_CXX_STANDARD})
    endif ()

    # Add template
    project(${_target} VERSION ${_version} LANGUAGES CXX)

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    add_executable(${_target} ${FUNC_SOURCES})

    foreach (_lib ${_qt_libs})
        target_link_libraries(${_target} PRIVATE ${_lib})
    endforeach ()

    foreach (_lib ${FUNC_LIBRARIES})
        target_link_libraries(${_target} PRIVATE ${_lib})
    endforeach ()

    foreach (_inc ${_qt_incs})
        target_include_directories(${_target} PRIVATE ${_inc})
    endforeach ()

    foreach (_inc ${FUNC_INCLUDE_DIRS})
        target_include_directories(${_target} PRIVATE ${_inc})
    endforeach ()

    if (FUNC_INCLUDE_CURRENT)
        target_include_directories(${_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    endif ()

    if (FUNC_WIN32_EXE)
        set_target_properties(${_target} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif ()

    # Unset temp vars
    unset(_qt_libs)
    unset(_qt_incs)

    unset(_version)
    unset(_cxx_standard)
endmacro()
