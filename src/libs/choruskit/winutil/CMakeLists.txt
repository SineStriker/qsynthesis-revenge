project(CKWinUtil VERSION 0.0.0.0 LANGUAGES CXX)

add_files(_src
    CURRENT
    PATTERNS *.h *.cpp
)

add_library(${PROJECT_NAME} STATIC ${_src})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CHORUSKIT_SHARED_INCDIR})
