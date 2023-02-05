project(MidiTest
    LANGUAGES CXX
    VERSION 0.0.0.1
)

set(_dirs)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME} ENABLE_SHARED NO_DEPLOY)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS DsCore
    INCLUDES ${COMMON_INCLUDE_DIR} ${_dirs}
)

qs_add_executable(${PROJECT_NAME}_Main ${PROJECT_NAME} NO_DEPLOY)

if(APP_DEPLOY)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIR})
endif()