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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(FSNOTIFIER_SOURCES ${_internal_src})

if (FSNOTIFIER_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${FSNOTIFIER_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC FSNOTIFIER_STATIC)
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
    get_target_property(WIN32_EXPORT_NAME ${PROJECT_NAME} OUTPUT_NAME)
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "Sine Striker")
    set(WIN32_FILE_DESC "C++ Wrapper for IDEA FSNotifier")
    set(WIN32_PRODUCT_NAME "fsnotifier")
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()

add_subdirectory(tools)
