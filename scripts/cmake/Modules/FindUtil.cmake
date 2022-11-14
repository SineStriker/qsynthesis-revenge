# Use this micro to find a Qt module
macro(find_qt_module)
    foreach(_module ${ARGV})
        find_package(QT NAMES Qt6 Qt5 COMPONENTS ${_module} REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${_module} REQUIRED)
    endforeach()
endmacro()

# Use this micro to include a Qt module
macro(add_qt_module _list)
    foreach(_module ${ARGN})
        find_qt_module(${_module})
        list(APPEND ${_list} Qt${QT_VERSION_MAJOR}::${_module})
    endforeach()
endmacro()

# Use this micro to include a Qt private header
macro(add_qt_private_inc _list)
    foreach(_module ${ARGN})
        list(APPEND ${_list} ${Qt${QT_VERSION_MAJOR}${_module}_PRIVATE_INCLUDE_DIRS})
    endforeach()
endmacro()
