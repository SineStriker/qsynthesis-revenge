project(LvCore LANGUAGES CXX VERSION ${INNER_VERSION})

add_files(_src
    CURRENT
    DIRECTORIES Api Basic Commands Render Serialization Utils
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_lv_mod(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDE_CURRENT
    WIN32_FILE_DESC "LabelVoice Core Module"
    WIN32_PRODUCT_NAME "LabelVoice.Core"
)

target_link_libraries(${PROJECT_NAME} PRIVATE yaml-cpp)

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_VERSION="${PROJECT_VERSION_SHORT}")
