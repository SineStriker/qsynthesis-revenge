project(tst_VstTest VERSION 0.0.0.0)

set(CMAKE_CXX_STANDARD 20)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

ck_add_test(${PROJECT_NAME} AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network RemoteObjects
    LINKS QsFramework DiffScope::vstbridge
)