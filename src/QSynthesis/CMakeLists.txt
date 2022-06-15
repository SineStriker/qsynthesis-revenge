# ------------------------------ Basic information ------------------------------

set (CURRENT_PROJECT_NAME ${PROJECT_NAME})

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

# Use this micro to find a Qt module
macro(find_qt_module Module)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS ${Module} REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${Module} REQUIRED)
endmacro()

# Use this micro to include a Qt module
macro(add_qt_module List Module)
    find_qt_module(${Module})
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

# Use to add files recursively
macro(add_recursively List)
    set(TEMP_LIST)
    file(GLOB_RECURSE TEMP_LIST ${ARGN})
    foreach (Item ${TEMP_LIST})
        list(APPEND ${List} ${Item})
    endforeach ()
    set(TEMP_LIST)
endmacro()


# ------------------------------ Include/Import/Link ------------------------------

add_qt_module(QT_LIBS Widgets)
add_qt_module(QT_LIBS Svg)
add_qt_module(QT_LIBS Network)
add_qt_module(QT_LIBS Multimedia)

include_qt_private(Gui)
include_qt_private(Widgets)

# Qt resources file (.qrc)
add_files(QT_RESOURCES Resources/res.qrc)

# Add resource files
if (SYS_WIN)
    # Add windows resource file
    add_files(RC_FILES Resources/logo/win/res.rc)
elseif (SYS_MAC)
    # Add mac icon file
    add_files(ICONS Resources/logo/mac/qsynth2.icns)
endif ()

# Add fixed files
add_recursively(PROJECT_SOURCES "${MY_SRC_DIR}/Framework/*.cpp" "${MY_SRC_DIR}/Framework/*.cc")
add_recursively(PROJECT_HEADERS "${MY_SRC_DIR}/Framework/*.hpp" "${MY_SRC_DIR}/Framework/*.h")
add_recursively(PROJECT_SOURCES "${MY_SRC_DIR}/Meta/*.cpp" "${MY_SRC_DIR}/Meta/*.cc")
add_recursively(PROJECT_HEADERS "${MY_SRC_DIR}/Meta/*.hpp" "${MY_SRC_DIR}/Meta/*.h")
add_recursively(PROJECT_SOURCES "${MY_SRC_DIR}/Vendor/*.cpp" "${MY_SRC_DIR}/Vendor/*.cc")
add_recursively(PROJECT_HEADERS "${MY_SRC_DIR}/Vendor/*.hpp" "${MY_SRC_DIR}/Vendor/*.h")
add_recursively(PROJECT_SOURCES "${MY_SRC_DIR}/Staging/*.cpp" "${MY_SRC_DIR}/Staging/*.cc")
add_recursively(PROJECT_HEADERS "${MY_SRC_DIR}/Staging/*.hpp" "${MY_SRC_DIR}/Staging/*.h")

# Add specific files
add_files(PROJECT_SOURCES
        main.cpp
        )

add_files(PROJECT_HEADERS
        )

# Add executable
if (SYS_WIN)
    # Windows need rc files
    add_executable(${CURRENT_PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES} ${RC_FILES})
elseif (SYS_MAC)
    # Mac need icon
    add_executable(${CURRENT_PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES} ${ICONS})
else ()
    # Linux
    add_executable(${CURRENT_PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${QT_RESOURCES})
endif ()

# Add Definitions
target_compile_definitions(${CURRENT_PROJECT_NAME} PRIVATE APP_VERSION="${APP_VERSION}")

# Link OS dependent libraries
if (SYS_WIN)
    target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE dwmapi user32 shell32 advapi32 winmm)
elseif (SYS_MAC)
    target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE CoreMIDI CoreFoundation CoreAudio)
else ()
    target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE asound)
endif ()

# Set Properties
if (APP_DEPLOY)
    set_target_properties(${CURRENT_PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_VERSION 1.0.0.0
            MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
            )
endif()

# Link Custom libraries
target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE FramelessHelper::Widgets)

target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE QMidi)

# Link Qt libraries
target_link_libraries(${CURRENT_PROJECT_NAME} PRIVATE ${QT_LIBS})

# ------------------------------ Translations ------------------------------

set (TS_FILES)
set (QM_FILES)

find_qt_module(LinguistTools)

# Simplified Chinese
add_files(TS_FILES Resources/translations/qsynthesis_zh_CN.ts)

set_source_files_properties(${TS_FILES}
    PROPERTIES OUTPUT_LOCATION "${MY_SRC_DIR}/Resources/translations"
    )
qt5_create_translation(QM_FILES ${MY_SRC_DIR} ${TS_FILES})

# ------------------------------ Promote code experience ------------------------------

# Include directories

add_files(INCLUDE_PATHS
    Framework
    Framework/Custom
    Framework/Helpers
    Framework/Metatypes
    Framework/Metatypes/Core
    Framework/Metatypes/User
    Framework/Visual
    Meta
    Staging
    Staging/Application
    Staging/Application/Managers
    Staging/Backend/Basic
    Staging/Main
    Staging/Main/Managers
    Staging/Main/Tabs
    Staging/Frontend/Basic
    Vendor/Frontend
    Vendor/Frontend/Utils
    Vendor/Frontend/Window
    Vendor/Backend
    Vendor/Backend/Basic
    Vendor/Backend/Freq
    Vendor/Backend/Import
    Vendor/Backend/Project
    Vendor/Backend/Utils
    Vendor/Backend/VoiceBank
    Vendor/LowLevel
    Vendor/LowLevel/Utau
    )

foreach (Path ${INCLUDE_PATHS})
    include_directories(${Path})
endforeach ()
