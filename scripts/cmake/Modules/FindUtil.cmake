#[[
    find_qt_module modules...

    args:
        modules: Qt module names to find
#]]
macro(find_qt_module)
    foreach(_module ${ARGV})
        find_package(QT NAMES Qt6 Qt5 COMPONENTS ${_module} REQUIRED)
        find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${_module} REQUIRED)
    endforeach()
endmacro()

#[[
    add_qt_module <list> modules...

    args:
        list:       list to accept Qt libraries
        modules:    Qt module names to find
#]]
macro(add_qt_module _list)
    foreach(_module ${ARGN})
        find_qt_module(${_module})
        list(APPEND ${_list} Qt${QT_VERSION_MAJOR}::${_module})
    endforeach()
endmacro()

#[[
    add_qt_private_inc <list> modules...

    args:
        list:       list to accept private directories of Qt libraries
        modules:    Qt module names to find
#]]
macro(add_qt_private_inc _list)
    foreach(_module ${ARGN})
        list(APPEND ${_list} ${Qt${QT_VERSION_MAJOR}${_module}_PRIVATE_INCLUDE_DIRS})
    endforeach()
endmacro()
