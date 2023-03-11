project(CKWinUtil VERSION 0.0.0.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC utils_win.h utils_win.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CHORUSKIT_SHARED_INCDIR})
