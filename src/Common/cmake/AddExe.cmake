include_guard(DIRECTORY)

#[[
    qs_add_executable(
        <target>
        <dll>
        AUTHOR_NAME         <var>
        FILE_DESC           <var>
        PRODUCT_NAME        <var>
        EXECUTABLE_TYPE     <var>
        ICO_FILE            <var>
        ICNS_FILE           <var>
    )

    flags:
        NO_DEPLOY:       ignore metadata and skip it when deploying
        FORCE_LINK:      implicitly link the dll forcefully
        FORCE_CONSOLE:   build as a console application forcefully

    usage:
        1. set APP_LIB_DIR by project configuration
        2. set APP_DLL as the target file name of <dll>
        3. add an executable with metadata

]] #
function(qs_add_executable _target _dll)
    set(options NO_DEPLOY FORCE_LINK FORCE_CONSOLE)
    set(oneValueArgs AUTHOR_NAME FILE_DESC PRODUCT_NAME EXECUTABLE_TYPE ICO_FILE ICNS_FILE)
    set(multiValueArgs)

    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_winmain_target ${_target})
    get_property(_winmain_src GLOBAL PROPERTY WIN_MAIN_SRC)

    # ----------------- Template Begin -----------------

    # Add target
    add_executable(${_winmain_target} ${_winmain_src})

    target_include_directories(${_winmain_target} PRIVATE ${COMMON_INCLUDE_DIR})

    target_compile_definitions(${_winmain_target} PRIVATE APP_LIB_DIR="${APP_LIB_DIR}")

    if(WIN32 AND APP_DEPLOY AND NOT FUNC_FORCE_LINK)
        target_compile_definitions(${_winmain_target} PRIVATE APP_DLL="$<TARGET_FILE_NAME:${_dll}>")
        add_dependencies(${_winmain_target} ${_dll})
    else()
        target_compile_definitions(${_winmain_target} PRIVATE APP_ENABLE_ENTRY)
        target_link_libraries(${_winmain_target} PRIVATE ${_dll})
    endif()

    # if(WIN32)
    # # https://blog.csdn.net/qq_58286297/article/details/119611363
    # target_link_libraries(${_winmain_target} PRIVATE ComCtl32)
    # endif()
    if(NOT FUNC_NO_DEPLOY)
        if(NOT FUNC_PRODUCT_NAME)
            set(_product_name ${_target})
        else()
            set(_product_name ${FUNC_PRODUCT_NAME})
        endif()

        set(_icns_file ${FUNC_ICNS_FILE})
        set(_ico_file ${FUNC_ICO_FILE})

        # Add embedded resources
        if(WIN32)
            # Set windows/console application
            if((APP_DEPLOY AND NOT CONFIG_WIN32_DEBUG AND NOT FUNC_FORCE_CONSOLE) OR NOT DEFINED NOT_QT_CREATOR)
                set_target_properties(${_target} PROPERTIES
                    WIN32_EXECUTABLE TRUE
                )
            endif()

            # configure manifest
            set(WIN32_MANIFEST_IDENTIFIER "${_product_name}")
            set(WIN32_MANIFEST_DESC "${FUNC_FILE_DESC}")
            configure_file(
                ${WIN32_EXE_MANIFEST}
                ${CMAKE_CURRENT_BINARY_DIR}/app.manifest
                @ONLY
            )

            target_sources(${_target} PRIVATE
                ${CMAKE_CURRENT_BINARY_DIR}/app.manifest
            )

            get_filename_component(_ico_name ${_ico_file} NAME)

            # configure rc
            set(WIN32_ICON_NAME ${_ico_name})
            set(WIN32_EXPORT_NAME ${_target})
            set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
            set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
            set(WIN32_AUTHOR_NAME "${FUNC_AUTHOR_NAME}")
            set(WIN32_FILE_DESC "${FUNC_FILE_DESC}")
            set(WIN32_PRODUCT_NAME "${_product_name}")
            configure_file(
                ${WIN32_EXE_RC}
                ${CMAKE_CURRENT_BINARY_DIR}/res.rc
                @ONLY
            )

            # copy ico
            file(COPY ${_ico_file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

            # add files
            target_sources(${_target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
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
                MACOSX_BUNDLE_BUNDLE_NAME ${_target}
                MACOSX_BUNDLE_EXECUTABLE_NAME ${_target}
                MACOSX_BUNDLE_ICON_FILE ${_icns_name}
                MACOSX_BUNDLE_INFO_STRING ${FUNC_FILE_DESC}
                MACOSX_BUNDLE_GUI_IDENTIFIER "${_product_name}"
                MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
                MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                MACOSX_BUNDLE_COPYRIGHT "Copyright (c) ${FUNC_AUTHOR_NAME}"
            )
        endif()

        set_target_properties(
            ${PROJECT_NAME}
            PROPERTIES
            TC_TARGET_TYPE EXECUTABLE
            TC_EXECUTABLE_TYPE ${FUNC_EXECUTABLE_TYPE}
        )
    endif()

    # ----------------- Template End -----------------
endfunction()
