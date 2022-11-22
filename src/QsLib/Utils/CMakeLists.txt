project(QsUtils
    VERSION 0.2.17.0
    LANGUAGES CXX
)

add_files(_src
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    QT_PRIVATE_INCLUDES Core
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Common Utilities"
    WIN32_PRODUCT_NAME "QSynthesis.Library.Utils"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_MYCOMMANDLINEPARSER)

target_include_directories(${PROJECT_NAME} PUBLIC
    src/Helpers
)
