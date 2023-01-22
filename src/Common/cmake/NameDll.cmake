include_guard(DIRECTORY)

#[[
    qs_name_dll <target> [HINT <hint>]

    args:
        target:      target name
        hint:        name hint
    
    usage:
        set target output name refering to certain rules

]] #
function(qs_name_dll _name)
    set(options)
    set(oneValueArgs HINT)
    set(multiValueArgs)

    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(USE_HINT off)

    if(FUNC_HINT AND NOT("${FUNC_HINT}" STREQUAL "") AND USE_HINT)
        set_target_properties(${_name} PROPERTIES OUTPUT_NAME OpenVpi.${FUNC_HINT})
    else()
        string(TOLOWER ${_name} _lower)
        set_target_properties(${_name} PROPERTIES OUTPUT_NAME ${_lower}-qt${QT_VERSION_MAJOR})
    endif()
endfunction()
