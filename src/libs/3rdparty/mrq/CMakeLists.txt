project(mrq LANGUAGES C)

add_library(${PROJECT_NAME} STATIC mrq.h mrq.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
