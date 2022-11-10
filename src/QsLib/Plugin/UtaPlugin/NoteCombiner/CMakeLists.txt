project(NoteCombiner
    VERSION 0.0.0.1
    LANGUAGES CXX
)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add source files
set(QT_LIBS)
set(SOURCES)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Core)

file(GLOB _root_src ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(APPEND SOURCES ${_root_src})

set(_main_dirs)
foreach(_dir ${_main_dirs})
    set(_temp)
    file(GLOB_RECURSE _temp ${_dir}/*.h ${_dir}/*.cpp)
    list(APPEND SOURCES ${_temp})
    unset(_temp)
endforeach()

# Add library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC QsSVS)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add embedded resources
if (WIN32)
    # Specify your plugin metadata
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "Sine Striker")
    set(WIN32_FILE_DESC "QSynthesis Plugin for Utau Editor")
    set(WIN32_PRODUCT_NAME ${PROJECT_NAME})
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

# Add task to copy to plugins dir after build
settle_plugin(${PROJECT_NAME} utaplugins)