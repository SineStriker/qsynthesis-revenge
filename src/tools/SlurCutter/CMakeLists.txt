project(SlurCutter VERSION 0.0.1.2)

ck_add_files(_src
    CURRENT
    DIRS gui
    PATTERNS *.h *.cpp res.qrc
)

ck_add_application(${PROJECT_NAME} AUTOGEN
    DESCRIPTION "ChorusKit ${PROJECT_NAME}"
)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core Widgets Network
    LINKS QsFramework
)

ffmpeg_link_all(${PROJECT_NAME} PRIVATE)

target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSON="${PROJECT_VERSION}")

include(qasc_exec.cmake)