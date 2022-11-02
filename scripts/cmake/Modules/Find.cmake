# Use this micro to find a Qt module
macro(find_qt_module Module)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS ${Module} REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${Module} REQUIRED)
endmacro()


# Use this micro to include a Qt module
macro(add_qt_module List Module)
    find_qt_module(${Module})
    list(APPEND ${List} Qt${QT_VERSION_MAJOR}::${Module})
endmacro()


# Use this micro to include a Qt private header
macro(include_qt_private Module)
    include_directories(${Qt${QT_VERSION_MAJOR}${Module}_PRIVATE_INCLUDE_DIRS})
endmacro()
