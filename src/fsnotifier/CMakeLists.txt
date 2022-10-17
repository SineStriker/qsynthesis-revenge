project(fsnotifier-lib
    VERSION 0.0.0.1
    LANGUAGES CXX C
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(FSNOTIFIER_BUILD_STATIC off)

set(FSNOTIFIER_SOURCES)
set(QT_LIBS)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Core)

file(GLOB_RECURSE _internal_src 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(FSNOTIFIER_SOURCES ${_internal_src})

if (FSNOTIFIER_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${FSNOTIFIER_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC FSNOTIFIER_BUILD_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${FSNOTIFIER_SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE FSNOTIFIER_LIBRARY)

target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME fsnotifier-qt${QT_VERSION_MAJOR}
)

if (WIN32)
    get_target_property(PROJECT_EXPORT_FILENAME ${PROJECT_NAME} OUTPUT_NAME)
    configure_file(win32/res.rc.in
        ${PROJECT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR}/res.rc)
endif()

add_subdirectory(tools)