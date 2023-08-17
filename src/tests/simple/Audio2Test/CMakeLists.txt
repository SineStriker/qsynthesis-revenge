project(tst_Audio2Test VERSION 0.0.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(SndFile QUIET)
find_package(SampleRate QUIET)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

ck_add_test(${PROJECT_NAME} AUTOGEN)

if(WIN32)
    if(NOT ASIOSDK_DIR)
        message(STATUS "ASIOSDK_DIR not specified. ASIO Feature will be disabled.")
    else()
        target_compile_definitions(${PROJECT_NAME} PRIVATE USE_FEATURE_ASIO)
        target_include_directories(${PROJECT_NAME} PRIVATE ${ASIOSDK_DIR}/common)
    endif()
endif()

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets
    LINKS QsFramework
    QT_INCLUDES_PRIVATE Core
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE SndFile::sndfile)
target_link_libraries(${PROJECT_NAME} PRIVATE SampleRate::samplerate)