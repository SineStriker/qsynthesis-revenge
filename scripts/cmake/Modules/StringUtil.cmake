include_guard(DIRECTORY)

#[[
    string_split <var> <string> <separator>

    usage:
        split a string by separator and return the result into `var`
#]]
function(string_split OUTPUT str separator)
    set(list)

    # Test
    string(FIND ${str} ${separator} index)

    # Cycle
    while(${index} GREATER_EQUAL 0)
        # Add to list
        string(SUBSTRING ${str} 0 ${index} temp_str)
        list(APPEND list ${temp_str})

        # Calculate length
        string(LENGTH ${str} len)
        math(EXPR index "${index}+1")
        math(EXPR len "${len}-${index}")

        # Update rest string
        string(SUBSTRING ${str} ${index} ${len} str)
        string(FIND ${str} ${separator} index)
    endwhile()

    # Add last
    list(APPEND list ${str})

    # Return
    set(${OUTPUT} ${list} PARENT_SCOPE)
endfunction()
