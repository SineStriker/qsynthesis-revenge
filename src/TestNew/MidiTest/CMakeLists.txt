project(MidiTest
    LANGUAGES CXX
    VERSION 0.0.0.1
)

set(_dirs)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME} ENABLE_SHARED AS_TEST)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS DsGui
    INCLUDES ${COMMON_INCLUDE_DIR} ${_dirs}
)

qs_add_executable(${PROJECT_NAME}_Main ${PROJECT_NAME} FORCE_CONSOLE AS_TEST)
