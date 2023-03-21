project(ExtensionSystem VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

ck_add_files(_src CURRENT DIRS newSettings PATTERNS *.h *.cpp *.ui *.qrc)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets
    LINKS Aggregation Utils
)

configure_qtc_library(${PROJECT_NAME})

set_source_files_properties(pluginmanager.cpp PROPERTIES SKIP_AUTOMOC ON)