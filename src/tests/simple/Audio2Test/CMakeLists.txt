project(tst_Audio2Test VERSION 0.0.0.0)

set(CMAKE_CXX_STANDARD 20)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

ck_add_test(${PROJECT_NAME} AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core
    LINKS QsFramework
    QT_INCLUDES_PRIVATE Core
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)