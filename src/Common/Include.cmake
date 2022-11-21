# Include `Include.cmake` in all sub-directories
get_property(_subdirs DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY SUBDIRECTORIES)

foreach(_dir ${_subdirs})
    if(EXISTS ${_dir}/Include.cmake)
        include(${_dir}/Include.cmake)
    endif()
endforeach()

unset(_subdirs)
