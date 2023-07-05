project(vstglobal VERSION 0.0.0.0)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ./)