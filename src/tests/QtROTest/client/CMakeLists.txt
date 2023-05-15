project(QtROTestClient VERSION 0.0.0.0)

ck_find_qt_module(RemoteObjects)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

qt5_generate_repc(_src SimpleSwitch.rep REPLICA)

ck_add_test(${PROJECT_NAME} AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network RemoteObjects
    LINKS
    INCLUDE_PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)