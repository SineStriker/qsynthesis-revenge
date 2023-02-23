project(qtdummy VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ----------------------------------
# Add modules
# ----------------------------------
add_qt_module(_qt_libs Core Gui Widgets Network)

# ----------------------------------
# Add target
# ----------------------------------
add_files(_src CURRENT_RECURSE PATTERNS *.h *.c *.cpp)
add_executable(${PROJECT_NAME} ${_src})

# ----------------------------------
# Target...
# ----------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE ${_qt_libs})