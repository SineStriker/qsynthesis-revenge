project(VstTest VERSION 0.0.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp *.rep)

qt5_generate_repc(_src MyObject.rep SOURCE)

ck_add_test(${PROJECT_NAME})

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network RemoteObjects
    LINKS QsFramework DiffScope::ipcbridge DiffScope::vstserver
)