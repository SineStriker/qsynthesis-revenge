project(tst_AudioSlicerTest VERSION 0.0.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


ck_add_files(_src
    CURRENT
    DIRS slicer
    PATTERNS *.h *.cpp
)

ck_add_test(${PROJECT_NAME})

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets
    LINKS QsFramework
    INCLUDE_PRIVATE ${CK_HEADERS_OUTPUT_PATH}
)

find_package(SndFile CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)

ffmpeg_link_all(${PROJECT_NAME} PRIVATE)
