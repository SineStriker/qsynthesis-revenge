# ------------------------------ Basic information ------------------------------

# Include current directory
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Variables
set(QT_LIBS)
set(QT_RESOURCES)
set(RC_FILES)
set(ICONS)
set(MY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES)
set(PROJECT_HEADERS)
set(INCLUDE_PATHS)


# ------------------------------ Macros/Functions ------------------------------

# Use this micro to include a Qt module
macro(add_qt_module List Module)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS ${Module} REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${Module} REQUIRED)
    list(APPEND ${List} Qt${QT_VERSION_MAJOR}::${Module})
endmacro()

# Use this micro to include a Qt private header
macro(include_qt_private Module)
    include_directories(${Qt${QT_VERSION_MAJOR}${Module}_PRIVATE_INCLUDE_DIRS})
endmacro()

# Use this micro to add prefix to each item and save to list
macro(list_append_with_prefix List Prefix)
    foreach (Item ${ARGN})
        list(APPEND ${List} ${Prefix}${Item})
    endforeach ()
endmacro()

# Use this micro to remove all items in list1 which are in list2
macro(list_remove_all List1 List2)
    foreach (Item ${${List2}})
        list(REMOVE_ITEM ${List1} ${Item})
    endforeach ()
endmacro()

# Use to conveniently add files in this CMakeLists
macro(add_files List)
    list_append_with_prefix(${List} ${MY_SRC_DIR}/ ${ARGN})
endmacro()


# ------------------------------ Include/Import/Link ------------------------------

add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)
add_qt_module(QT_LIBS Network)

include_qt_private(Gui)
include_qt_private(Widgets)

add_files(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        )

add_files(PROJECT_HEADERS
        MainWindow.h
        )

# Add executable
if (SYS_WIN)
    # Windows need rc files
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES} ${RC_FILES})
elseif (SYS_MAC)
    # Mac need icon
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES} ${ICONS})
else ()
    # Linux
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
endif ()

# Link OS dependent libraries
if (SYS_WIN)
    target_link_libraries(${PROJECT_NAME} PRIVATE dwmapi user32 shell32 advapi32 winmm)
elseif (SYS_MAC)
    target_link_libraries(${PROJECT_NAME} PRIVATE CoreMIDI CoreFoundation CoreAudio)
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE asound)
endif ()

# Set Properties
set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        )

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

# Link Custom libraries
target_link_libraries(${PROJECT_NAME} PRIVATE FramelessHelper::Widgets)


# ------------------------------ Promote code experience ------------------------------

# Include directories

foreach (Path ${INCLUDE_PATHS})
    include_directories(${Path})
endforeach ()
