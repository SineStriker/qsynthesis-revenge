macro(settle_plugin _target _category)
    add_custom_command(TARGET ${_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory 
            $<TARGET_FILE_DIR:${_target}>/plugins/${_category}
        COMMAND ${CMAKE_COMMAND} -E copy 
            $<TARGET_FILE:${_target}> $<TARGET_FILE_DIR:${_target}>/plugins/${_category} 
    )
endmacro()

set(PLUGIN_WINRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/win32/res.rc.in)

add_subdirectory(CompressEngine)

add_subdirectory(UstConverter)

add_subdirectory(UtaPlugin)

add_subdirectory(WindowFactory)