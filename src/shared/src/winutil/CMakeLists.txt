project(shared_winutil)

add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_library(${PROJECT_NAME} STATIC ${_src})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SHARED_INCLUDE_DIR})
