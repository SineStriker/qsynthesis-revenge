# Usage:
# cmake
# -D arg="file1;new_file1;file2"
# -P RelativeCopy.cmake

# Copy file2 to a new position whose relative path to new_file1 is same as before

include(${CMAKE_CURRENT_LIST_DIR}/Modules/Basic.cmake)

check_defined(arg)

# Message
get_filename_component(SCRIPT_NAME ${CMAKE_CURRENT_LIST_FILE} NAME)
message("Run ${SCRIPT_NAME} Script")

# Get arguments
list(LENGTH arg _len)

if(NOT _len EQUAL 3)
    message(FATAL_ERROR "The number of arguments is incorrect.")
endif()

# Copy
list(GET arg 0 _file1)
list(GET arg 1 _new_file1)
list(GET arg 2 _file2)

get_filename_component(_org_dir ${_file1} DIRECTORY)
get_filename_component(_new_dir ${_new_file1} DIRECTORY)
file(RELATIVE_PATH _rel_path ${_org_dir} ${_file2})
set(_new_file2 ${_new_dir}/${_rel_path})
get_filename_component(_new_dir2 ${_new_file2} DIRECTORY)

if(NOT EXISTS ${_new_dir2})
    make_directory(${_new_dir2})
endif()

get_filename_component(_name ${_file2} NAME)
message("Copy relative: ${_name}")

file(COPY ${_file2} DESTINATION ${_new_dir2})