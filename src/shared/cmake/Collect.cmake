include_guard(DIRECTORY)

function(qs_add_collect _collection _var)
    get_property(_tmp GLOBAL PROPERTY GLOBAL_COLLECTOR_${_collection})
    list(APPEND _tmp ${_var})
    set_property(GLOBAL PROPERTY GLOBAL_COLLECTOR_${_collection} ${_tmp})
endfunction()

function(qs_get_collect _collection _out)
    get_property(_tmp GLOBAL PROPERTY GLOBAL_COLLECTOR_${_collection})
    set(${_out} ${_tmp} PARENT_SCOPE)
endfunction()