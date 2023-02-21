cmake_minimum_required(VERSION 3.17)

# Project
project(fsnotifier-qt
    VERSION 0.0.0.1
    LANGUAGES CXX C
)

# Set library configuration
set(FSNOTIFIER_BUILD_STATIC off)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ------------------------------ Configure Target Begin ------------------------------

# # Add source files
# set(_qt_libs)
# set(_sources)

# find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
# find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
# list(APPEND _qt_libs Qt${QT_VERSION_MAJOR}::Core)

# file(GLOB_RECURSE _internal_src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
# )

# list(APPEND _sources ${_internal_src})

# # Add library
# if(FSNOTIFIER_BUILD_STATIC)
#     add_library(${PROJECT_NAME} STATIC ${_sources})
#     target_compile_definitions(${PROJECT_NAME} PUBLIC FSNOTIFIER_STATIC)
# else()
#     add_library(${PROJECT_NAME} SHARED ${_sources})
# endif()

# target_compile_definitions(${PROJECT_NAME} PRIVATE FSNOTIFIER_LIBRARY)

# # Set library properties
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     OUTPUT_NAME fsnotifier-qt${QT_VERSION_MAJOR}
# )

# # Link libraries
# target_link_libraries(${PROJECT_NAME} PUBLIC ${_qt_libs})

# # Include directories
# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# # Add embedded resources
# if(WIN32)
#     string(TIMESTAMP _current_year %Y)

#     get_target_property(WIN32_EXPORT_NAME ${PROJECT_NAME} OUTPUT_NAME)
#     set(WIN32_COPYRIGHT_START_YEAR "2020")
#     set(WIN32_COPYRIGHT_END_YEAR "${_current_year}")
#     set(WIN32_AUTHOR_NAME "Sine Striker")
#     set(WIN32_FILE_DESC "C++ Wrapper for IDEA FSNotifier")
#     set(WIN32_PRODUCT_NAME "fsnotifier")
#     configure_file(
#         win32/Win32DllRes.rc.in
#         ${CMAKE_CURRENT_BINARY_DIR}/res.rc
#         @ONLY
#     )

#     target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
# elseif(APPLE)
#     set_target_properties(${_target} PROPERTIES
#         FRAMEWORK TRUE
#         FRAMEWORK_VERSION CXX
#         MACOSX_FRAMEWORK_NAME "fsnotifier"
#         MACOSX_FRAMEWORK_IDENTIFIER "fsnotifier"
#         MACOSX_FRAMEWORK_BUNDLE_VERSION ${PROJECT_VERSION}
#         MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     )
# endif()

# # For project collection
# set_target_properties(
#     ${PROJECT_NAME}
#     PROPERTIES
#     TC_TARGET_TYPE LIBRARY
#     TC_LIBRARY_TYPE Tool
#     TC_QT_BINARY true
# )

# ------------------------------ Configure Target End ------------------------------
add_subdirectory(tools)