project(QsFramework
    VERSION 0.2.17.0
    LANGUAGES CXX
)

# Build Options
set(QSFRAMEWORK_BUILD_STATIC off)

set(QT_LIBS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Widgets)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Svg REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Svg)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Network)

file(GLOB_RECURSE GLOBAL_INC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)
file(GLOB_RECURSE GLOBAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(PROJECT_SRC ${GLOBAL_INC} ${GLOBAL_SRC})

if (QSFRAMEWORK_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QSFRAMEWORK_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE QSFRAMEWORK_LIBRARY)

target_link_libraries(${PROJECT_NAME} PUBLIC ${QT_LIBS})

target_link_libraries(${PROJECT_NAME} PUBLIC QsUtils)

# SingleApplication Requirements
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE advapi32)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Custom
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Helpers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Metatypes
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Metatypes/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Metatypes/User
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Visual
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Qt${QT_VERSION_MAJOR}Widgets_PRIVATE_INCLUDE_DIRS}
)

if (WIN32)
    set(WIN32_EXPORT_NAME ${PROJECT_NAME})
    set(WIN32_COPYRIGHT_START_YEAR "${TIME_PROJECT_START_YEAR}")
    set(WIN32_COPYRIGHT_END_YEAR "${TIME_CURRENT_YEAR}")
    set(WIN32_AUTHOR_NAME "QSynthesis Team")
    set(WIN32_FILE_DESC "QSynthesis Common Utilities")
    set(WIN32_PRODUCT_NAME "QSynthesis.Framework")
    configure_file(
        ${WIN32_DLL_RC}
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()
