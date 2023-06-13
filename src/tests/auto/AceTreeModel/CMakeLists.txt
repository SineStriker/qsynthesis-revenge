project(tst_AceTreeModel)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

ck_add_test(${PROJECT_NAME} AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Test
    QT_INCLUDES_PRIVATE Core Widgets
    LINKS QMEditing
)