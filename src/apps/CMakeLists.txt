ck_get_subdirs(_dirs)

if(CK_BUILD_ALL_APPLICATIONS)
    foreach(_dir ${_dirs})
        add_subdirectory(${_dir})
    endforeach()
else()
    set(_apps_lower)

    foreach(_item ${CK_BUILD_APPLICATIONS})
        string(TOLOWER ${_item} _lower)
        list(APPEND _apps_lower ${_lower})
    endforeach()

    foreach(_dir ${_dirs})
        string(TOLOWER ${_dir} _lower)
        list(FIND _apps_lower ${_lower} _out)

        if(_out GREATER_EQUAL 0)
            add_subdirectory(${_dir})
        endif()
    endforeach()
endif()