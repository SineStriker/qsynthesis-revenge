project(QMidi)

set(BUILD_STATIC ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# Common QMidi source files & library
#aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SOURCES)
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(LIBRARIES Qt${QT_VERSION_MAJOR}::Core)

# Platform specific QMidi source files & libraries
if (WIN32)
    set(LIBRARIES ${LIBRARIES} winmm)
    set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/OS/QMidi_Win32.cpp")
elseif (APPLE)
    set(LIBRARIES ${LIBRARIES} "-framework CoreMIDI" "-framework CoreFoundation" "-framework CoreAudio")
    set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/OS/QMidi_CoreMidi.cpp")
elseif (UNIX)
    set(LIBRARIES ${LIBRARIES} asound)
    set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/OS/QMidi_ALSA.cpp")
elseif (HAIKU)
    set(LIBRARIES ${LIBRARIES} midi2)
    set(SOURCES ${SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/src/OS/QMidi_Haiku.cpp")
endif ()

# Add library
if (BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC QMIDI_STATIC)
else ()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}-Qt${QT_VERSION_MAJOR}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QMIDI_LIBRARY)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBRARIES})
