project(QsUtils
    VERSION 0.2.17.0
    LANGUAGES CXX
)

# Build Options
set(QSUTILS_BUILD_STATIC off)

set(QT_LIBS)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
list(APPEND QT_LIBS Qt${QT_VERSION_MAJOR}::Core)

file(GLOB_RECURSE GLOBAL_INC ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp)
file(GLOB_RECURSE GLOBAL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(PROJECT_SRC ${GLOBAL_INC} ${GLOBAL_SRC})

if (QSUTILS_BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QSUTILS_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SRC})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE QSUTILS_LIBRARY)

target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Helpers
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Qt${QT_VERSION_MAJOR}Core_PRIVATE_INCLUDE_DIRS}
)

if (WIN32)
    set(PROJECT_EXPORT_FILENAME ${PROJECT_NAME})
    configure_file(win32/res.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/res.rc
        @ONLY
    )
    target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
endif()
