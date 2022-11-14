project(LvElem
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

add_files(_src
    CURRENT
    DIRECTORIES Kernel Styles
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_lv_mod(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Widgets Svg
    QT_PRIVATE_INCLUDES Gui Widgets
    LINKS QsUtils QsIntegrate LvCore
    INCLUDE_CURRENT
    WIN32_FILE_DESC "LabelVoice Elementary Module"
    WIN32_PRODUCT_NAME "LabelVoice.Elementary"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_VERSION="${PROJECT_VERSION_SHORT}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_PLUGINS_DIR="${APP_PLUGINS_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_RES_DIR="${APP_RES_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_RES_BUILTIN_DIR="${APP_RES_BUILTIN_DIR}")

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_RES_PLUGINS_DIR="${APP_RES_PLUGINS_DIR}")
