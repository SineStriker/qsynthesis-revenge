project(ExtensionSystem VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

ck_add_files(_src CURRENT DIRS newSettings PATTERNS *.h *.cpp *.ui *.qrc)

configure_qtc_library(${PROJECT_NAME} EXTENSIONSYSTEM_LIBRARY)

ck_target_components(${PROJECT_NAME}
    AUTO_INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core Widgets
    LINKS Aggregation Utils
)

set_source_files_properties(pluginmanager.cpp PROPERTIES SKIP_AUTOMOC ON)