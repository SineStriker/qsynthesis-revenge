project(vst VERSION 0.0.0.0)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ./)

add_subdirectory(vstplugin)

add_subdirectory(vstbridge)

add_subdirectory(vstglobal)

add_subdirectory(QSystemSemaphoreExtended)