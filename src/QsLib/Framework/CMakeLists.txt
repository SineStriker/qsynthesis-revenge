project(QsFramework
    VERSION 0.2.17.0
    LANGUAGES CXX
)

add_files(_src
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

qslib_configure_library(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Widgets Svg Network
    QT_PRIVATE_INCLUDES Widgets
    LINKS QsUtils
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Common Utilities"
    WIN32_PRODUCT_NAME "QSynthesis.Framework"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    src/Custom
    src/Helpers
    src/Kernel
    src/Metatypes
    src/Metatypes/Core
    src/Metatypes/User
    src/Visual
)
