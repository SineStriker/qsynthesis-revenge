# https://www.codeproject.com/Articles/7658/CSplash-A-Splash-Window-Class

project(CKWinSplash VERSION 0.0.0.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC Splash.h Splash.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CHORUSKIT_SHARED_INCDIR})
