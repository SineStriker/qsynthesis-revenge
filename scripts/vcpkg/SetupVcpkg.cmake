if(${CMAKE_VERSION} VERSION_LESS "3.19")
    return()
endif()

if(APPLE)
    return()
endif()

if(NOT Python_FOUND)
    return()
endif()

function(_fix_vcpkg_json)
    # Find config file
    find_file(_target_file Qt5Config.cmake ${Qt5_DIR} ${CMAKE_PREFIX_PATH})

    if(NOT _target_file)
        message(FATAL_ERROR "Qt5Config.cmake not found.")
    endif()

    get_filename_component(_qt5_dir ${_target_file} DIRECTORY)

    # Generate setup-vcpkg.json
    set(_setup_vcpkg_json ${CK_PROJECT_ROOT_DIR}/setup-vcpkg.json)

    # Remove if exists
    if(EXISTS ${ROOT_VCPKG_DIR})
        file(REMOVE_RECURSE ${ROOT_VCPKG_DIR})
    endif()

    if(NOT EXISTS ${_setup_vcpkg_json})
        execute_process(
            COMMAND ${Python_EXECUTABLE} setup-vcpkg.py --init
            WORKING_DIRECTORY ${CK_PROJECT_ROOT_DIR}
        )
    endif()

    # Write setup-vcpkg.json
    file(READ ${_setup_vcpkg_json} _data)
    string(JSON _data SET ${_data} "values" "Qt5ConfigDir" "value" "\"${_qt5_dir}\"")
    file(WRITE ${_setup_vcpkg_json} ${_data})

    # Run setup-vcpkg
    if(WIN32)
        # Open new console
        set(_bat ${CMAKE_BINARY_DIR}/tmp/tmp_init_vcpkg.bat)
        file(WRITE ${_bat} "echo Y | start /wait cmd /C \"cd /D \"${CK_PROJECT_ROOT_DIR}\" && \"${Python_EXECUTABLE}\" setup-vcpkg.py\"")
        execute_process(
            COMMAND ${_bat}
            WORKING_DIRECTORY ${CK_PROJECT_ROOT_DIR}
            RESULT_VARIABLE _res
            COMMAND_ERROR_IS_FATAL ANY
        )
        file(REMOVE ${_bat})
    else()
        execute_process(
            COMMAND ${Python_EXECUTABLE} setup-vcpkg.py
            WORKING_DIRECTORY ${CK_PROJECT_ROOT_DIR}
            OUTPUT_FILE "/proc/self/fd/0"
            RESULT_VARIABLE _res
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    # Check result
    if(_res)
        file(REMOVE_RECURSE ${ROOT_VCPKG_DIR})
        message(FATAL_ERROR "Vcpkg init failed.")
    endif()
endfunction()

_fix_vcpkg_json()