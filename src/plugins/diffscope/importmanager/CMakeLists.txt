project(diffscope_importmanager VERSION 0.0.0.0)

add_files(_src CURRENT PATTERNS *.h *.cpp)

ck_add_plugin(${PROJECT_NAME}
    AUTHOR_NAME "Sine Striker"
    FILE_DESC "DiffScope Core Plugin"
    PRODUCT_NAME ${PROJECT_NAME}
    PARENT DiffScope
    SUBDIR plugins
    OUTPUT_NAME importmanager
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS QsFramework ExtensionSystem
)
