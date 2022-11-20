#[[
    qs_add_executable <target> <dll> [EXTERN_C]
    1. set APP_LIB_DIR by project configuration
    2. set APP_DLL as the target file name of <dll>
    3. add an executable with name of <target>
]]#
function(qs_add_executable _target _dll)
    set(options EXTERN_C)
    set(oneValueArgs)
    set(multiValueArgs)

    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_files ${FUNC_UNPARSED_ARGUMENTS})

    get_property(_winmain_src GLOBAL PROPERTY WIN_MAIN_SRC)

    set(_winmain_target ${_target})

    # Add WinMain
    add_executable(${_target} ${_winmain_src})

    target_compile_definitions(${_winmain_target} PRIVATE APP_LIB_DIR="${APP_LIB_DIR}")

    if(WIN32 AND APP_DEPLOY)
        target_compile_definitions(${_winmain_target} PRIVATE DELAY_LOAD)
        target_compile_definitions(${_winmain_target} PRIVATE APP_DLL="$<TARGET_FILE_NAME:${_dll}>")
        add_dependencies(${_winmain_target} ${_dll})
    else()
        target_compile_definitions(${_winmain_target} PRIVATE APP_ENABLE_ENTRY)
        target_link_libraries(${_winmain_target} PRIVATE ${_dll})
    endif()

    # if(WIN32)
    #     # https://blog.csdn.net/qq_58286297/article/details/119611363
    #     target_link_libraries(${_winmain_target} PRIVATE ComCtl32)
    # endif()

    if(FUNC_EXTERN_C)
        target_compile_definitions(${_winmain_target} PRIVATE APP_ENABLE_C_STYLE)
    endif()
endfunction()
