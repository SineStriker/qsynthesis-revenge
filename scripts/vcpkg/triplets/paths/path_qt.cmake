# Read qt path from txt file
file(STRINGS ${CMAKE_CURRENT_LIST_DIR}/path_qt.txt _qt_dir_file ENCODING UTF-8)

# Get first element
list(GET _qt_dir_file 0 _qt_dir)

set(ENV{QT_DIR} ${_qt_dir})
set(ENV{Qt5_DIR} ${_qt_dir})
set(ENV{Qt6_DIR} ${_qt_dir})
