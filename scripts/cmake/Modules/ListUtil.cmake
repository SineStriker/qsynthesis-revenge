# Use this micro to add prefix to each item and save to list
macro(list_append_with_prefix List Prefix)
    foreach(Item ${ARGN})
        list(APPEND ${List} ${Prefix}${Item})
    endforeach()
endmacro()


# Use this micro to remove all items in list1 which are in list2
macro(list_remove_all List1 List2)
    foreach(Item ${${List2}})
        list(REMOVE_ITEM ${List1} ${Item})
    endforeach()
endmacro()


# Use to add items recursively
macro(list_add_recursively List)
    set(TEMP_LIST)
    file(GLOB_RECURSE TEMP_LIST ${ARGN})

    foreach(Item ${TEMP_LIST})
        list(APPEND ${List} ${Item})
    endforeach()

    set(TEMP_LIST)
endmacro()