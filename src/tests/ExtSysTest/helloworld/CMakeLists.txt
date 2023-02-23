project(testplugin_helloworld VERSION 0.0.0.0)

add_files(_src CURRENT PATTERNS *.h *.cpp)

qs_add_plugin(${PROJECT_NAME}
    PARENT ExtSysTest
    SUBDIR plugins
    OUTPUT_NAME helloworld
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS ExtensionSystem
)