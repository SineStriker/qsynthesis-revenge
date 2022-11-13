project(QsIntegrate
    VERSION 0.0.1.0
    LANGUAGES CXX
)

# Library type
set(QSINTEGRATE_BUILD_STATIC off)

# Qt Auto Compiler
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# ------------------------------ Configure Target Begin ------------------------------

# Add source files
set(QT_LIBS)
set(QT_INCS)
set(SOURCES)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

list(APPEND QT_INCS
    ${Qt${QT_VERSION_MAJOR}Core_PRIVATE_INCLUDE_DIRS}
    ${Qt${QT_VERSION_MAJOR}Widgets_PRIVATE_INCLUDE_DIRS}
)

set(_temp)
file(GLOB_RECURSE _temp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
list(APPEND SOURCES ${_temp})
unset(_temp)

# Add library
if (QSINTEGRATE_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QSINTEGRATE_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE QSINTEGRATE_LIBRARY)

# Link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC QsFramework QsMedia QsSVS)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${QT_INCS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add embedded resources
if (WIN32)
    # configure rc
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "QSynthesis Team")
    set(WIN32_FILE_DESC "QSynthesis Integrated Utilities")
    set(WIN32_PRODUCT_NAME "QSynthesis.Integrate")
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()
