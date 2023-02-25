project(ExtensionSystem VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_files(_src CURRENT PATTERNS *.h *.cpp *.ui *.qrc)

# ----------------------------------
# Build shared as LGPL requires
# ----------------------------------
configure_qtc_library(${PROJECT_NAME})

# ----------------------------------
# Add components
# ----------------------------------
target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Widgets
    LINKS Aggregation Utils
)

set_source_files_properties(pluginmanager.cpp PROPERTIES SKIP_AUTOMOC ON)