include_guard(DIRECTORY)

#[[
    ck_add_executable(
        <target>
        <dll>
        AUTHOR_NAME         <var>
        FILE_DESC           <var>
        PRODUCT_NAME        <var>
        EXECUTABLE_TYPE     <var>
        ICO_FILE            <var>
        ICNS_FILE           <var>

        [OUTPUT_NAME        <var>]
        [COPYRIGHT_YEAR     <var>]

        [APP_NAME_HINT]     <var>]
        [APP_ORG_HINT]      <var>]
        [APP_DOMAIN_HINT]   <var>]
        [APP_CORE_HINT]     <var>]
    )

    flags:
        AS_TEST:         ignore metadata and skip it when deploying
        FORCE_CONSOLE:   build as a console application forcefully
        FORCE_WINDOWS:   build as a windows application forcefully
        NO_EXTRA_TARGET: skip building another executable for windows

    usage:
        1. set APP_BIN_DIR by project configuration
        2. set APP_DLL as the target file name of <dll>
        3. add an executable with metadata

]] #
function(ck_add_executable _target _dll)
    set(options AS_TEST FORCE_CONSOLE FORCE_WINDOWS NO_EXTRA_TARGET)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME EXECUTABLE_TYPE ICO_FILE ICNS_FILE
        OUTPUT_NAME COPYRIGHT_YEAR APP_NAME_HINT APP_VERSION_HINT
    )
    set(multiValueArgs)

    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_target_list)

    # ----------------- Template Begin -----------------
    get_property(_appmain_src GLOBAL PROPERTY CHORUSKIT_WINMAIN_SRC)

    if(TRUE)
        add_executable(${_target} ${_appmain_src})
        list(APPEND _target_list ${_target})

        target_include_directories(${_target} PRIVATE ${CHORUSKIT_SHARED_INCDIR})
        target_compile_definitions(${_target} PRIVATE APP_ENABLE_ENTRY)
        target_link_libraries(${_target} PRIVATE ${_dll})
    endif()

    # Add extra target for Windows
    if(WIN32 AND NOT FUNC_NO_EXTRA_TARGET)
        set(_appmain_target ${_target}_appmain)
        add_executable(${_appmain_target} ${_appmain_src})
        list(APPEND _target_list ${_appmain_target})

        target_include_directories(${_appmain_target} PRIVATE ${CHORUSKIT_SHARED_INCDIR})
        target_link_libraries(${_appmain_target} PRIVATE CKWinUtil)
        target_compile_definitions(${_appmain_target} PRIVATE APP_BIN_DIR="bin")
        target_compile_definitions(${_appmain_target} PRIVATE APP_DLL="$<TARGET_FILE_NAME:${_dll}>")

        add_dependencies(${_appmain_target} ${_dll})

        # https://blog.csdn.net/qq_58286297/article/details/119611363
        # target_link_libraries(${_appmain_target} PRIVATE ComCtl32)
        set_target_properties(${_appmain_target} PROPERTIES OUTPUT_NAME ${_target})
        set_target_properties(${_appmain_target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CONFIG_OUTPUT_DIRECTORY})
    endif()

    # Add compile definitions
    foreach(_item ${_target_list})
        if(FUNC_APP_NAME_HINT)
            target_compile_definitions(${_item} PRIVATE APP_NAME_HINT="${FUNC_APP_NAME_HINT}")
        endif()

        if(FUNC_APP_VERSION_HINT)
            target_compile_definitions(${_item} PRIVATE APP_VERSION_HINT="${FUNC_APP_VERSION_HINT}")
        endif()
    endforeach()

    if(FUNC_OUTPUT_NAME)
        set(_output_name ${FUNC_OUTPUT_NAME})

        foreach(_item ${_target_list})
            set_target_properties(${_item} PROPERTIES OUTPUT_NAME ${_output_name})
        endforeach()
    else()
        set(_output_name ${_target})
    endif()

    # Set windows/console application
    if((NOT CONFIG_WIN32_DEBUG AND NOT FUNC_FORCE_CONSOLE) OR NOT DEFINED NOT_QT_CREATOR OR FUNC_FORCE_WINDOWS)
        foreach(_item ${_target_list})
            set_target_properties(${_item} PROPERTIES
                WIN32_EXECUTABLE TRUE
            )
        endforeach()
    endif()

    if(NOT FUNC_AS_TEST)
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

        set(_icns_file ${FUNC_ICNS_FILE})
        set(_ico_file ${FUNC_ICO_FILE})

        # Add embedded resources
        if(WIN32)
            if(CONFIG_CMAKE_RANDOM_CONFIGURE_FILE)
                string(RANDOM LENGTH 8 _rand)
                set(manifest_name ${CMAKE_CURRENT_BINARY_DIR}/app_${_rand}.manifest)
                set(rc_name ${CMAKE_CURRENT_BINARY_DIR}/res_${_rand}.rc)
            else()
                set(manifest_name ${CMAKE_CURRENT_BINARY_DIR}/app.manifest)
                set(rc_name ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
            endif()

            # configure manifest
            set(WIN32_MANIFEST_IDENTIFIER "${_product_name}")
            set(WIN32_MANIFEST_DESC "${FUNC_FILE_DESC}")
            configure_file(
                ${WIN32_EXE_MANIFEST}
                ${manifest_name}
                @ONLY
            )

            # configure rc
            get_filename_component(_ico_name ${_ico_file} NAME)

            set(WIN32_ICON_NAME ${_ico_name})
            set(WIN32_EXPORT_NAME ${_output_name})
            set(WIN32_COPYRIGHT_YEAR "${_copyright_year}")
            set(WIN32_AUTHOR_NAME "${FUNC_AUTHOR_NAME}")
            set(WIN32_FILE_DESC "${FUNC_FILE_DESC}")
            set(WIN32_PRODUCT_NAME "${_product_name}")
            configure_file(
                ${WIN32_EXE_RC}
                ${rc_name}
                @ONLY
            )

            # copy ico
            file(COPY ${_ico_file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

            # add files
            foreach(_item ${_target_list})
                target_sources(${_item} PRIVATE
                    ${manifest_name}
                    ${rc_name}
                )
            endforeach()
        elseif(APPLE)
            # And this part tells CMake where to find and install the file itself
            set_source_files_properties(${_icns_file} PROPERTIES
                MACOSX_PACKAGE_LOCATION "Resources"
            )

            # NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
            # the property added to Info.plist
            get_filename_component(_icns_name ${_icns_file} NAME)

            # configure mac plist
            set_target_properties(${_target} PROPERTIES
                MACOSX_BUNDLE TRUE
                MACOSX_BUNDLE_BUNDLE_NAME ${_output_name}
                MACOSX_BUNDLE_EXECUTABLE_NAME ${_output_name}
                MACOSX_BUNDLE_ICON_FILE ${_icns_name}
                MACOSX_BUNDLE_INFO_STRING ${FUNC_FILE_DESC}
                MACOSX_BUNDLE_GUI_IDENTIFIER "${_product_name}"
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                MACOSX_BUNDLE_COPYRIGHT "Copyright (c) ${FUNC_AUTHOR_NAME}"
            )

            # ICNS icon MUST be added to executable's sources list, for some reason
            # Only apple can do
            target_sources(${_target} PRIVATE ${_icns_file})
        endif()
    endif()

    # ----------------- Template End -----------------
endfunction()
