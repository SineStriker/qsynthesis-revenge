#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

foreach(_target ${ffmpeg_subprojects})

    if (WIN32)
        file(GLOB _lib ${_IMPORT_PREFIX}/lib/*${_target}*.lib)
        if ("${_lib}" STREQUAL "")
            message(FATAL_ERROR "Failed to find definition file of ${_target}.")
        endif()

        file(GLOB _dll ${_IMPORT_PREFIX}/bin/*${_target}*.dll)
        if ("${_dll}" STREQUAL "")
            message(FATAL_ERROR "Failed to find shared library of ${_target}.")
        endif()

        set_property(TARGET FFmpeg::${_target} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(FFmpeg::${_target} PROPERTIES
            IMPORTED_IMPLIB_RELEASE ${_lib}
            IMPORTED_LOCATION_RELEASE ${_dll}
        )

        list(APPEND _IMPORT_CHECK_TARGETS FFmpeg::${_target})
        list(APPEND _IMPORT_CHECK_FILES_FOR_FFmpeg::${_target} ${_lib} ${_dll})
    elseif (APPLE)
        file(GLOB _dylib ${_IMPORT_PREFIX}/lib/*${_target}*.dylib)
        if ("${_dylib}" STREQUAL "")
            message(FATAL_ERROR "Failed to find definition file of ${_target}.")
        endif()

        get_filename_component(_name ${_dylib} NAME)
        if (IS_SYMLINK ${_dylib})
            file(READ_SYMLINK ${_dylib} _symlink)
            if (IS_ABSOLUTE ${_symlink})
                set(_dylib ${_symlink})
            else()
                set(_dylib ${_IMPORT_PREFIX}/lib/${_symlink})
            endif()
        endif()

        set_property(TARGET FFmpeg::${_target} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(FFmpeg::${_target} PROPERTIES
            IMPORTED_LOCATION_RELEASE ${_dylib}
            IMPORTED_SONAME_RELEASE ${_name}
        )

        list(APPEND _IMPORT_CHECK_TARGETS FFmpeg::${_target})
        list(APPEND _IMPORT_CHECK_FILES_FOR_FFmpeg::${_target} ${_dylib})
    else()
        file(GLOB _so ${_IMPORT_PREFIX}/lib/*${_target}*.so)
        if ("${_so}" STREQUAL "")
            message(FATAL_ERROR "Failed to find shared library of ${_target}.")
        endif()

        get_filename_component(_name ${_so} NAME)
        if (IS_SYMLINK ${_so})
            file(READ_SYMLINK ${_so} _symlink)
            if (IS_ABSOLUTE ${_symlink})
                set(_so ${_symlink})
            else()
                set(_so ${_IMPORT_PREFIX}/lib/${_symlink})
            endif()
        endif()

        set_property(TARGET FFmpeg::${_target} APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(FFmpeg::${_target} PROPERTIES
            IMPORTED_LOCATION_RELEASE ${_so}
            IMPORTED_SONAME_RELEASE ${_name}
        )

        list(APPEND _IMPORT_CHECK_TARGETS FFmpeg::${_target})
        list(APPEND _IMPORT_CHECK_FILES_FOR_FFmpeg::${_target} ${_so})

    endif()

endforeach()

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
