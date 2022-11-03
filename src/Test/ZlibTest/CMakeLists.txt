include(${PROJECT_MODULES_DIR}/Find.cmake)

project(ZlibTest)

# Add Qt Libs
set(QT_LIBS)

add_qt_module(QT_LIBS Core)

# Add Exe
add_executable(${PROJECT_NAME} main.cpp)

# Link Libs
target_link_libraries(${PROJECT_NAME} PRIVATE ${QT_LIBS})

target_link_libraries(${PROJECT_NAME} PRIVATE QsUtils)
