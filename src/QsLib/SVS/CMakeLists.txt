project(QsSVS
    VERSION 0.2.17.0
    LANGUAGES C CXX
)

add_subdirectory(tools)

add_files(_src
    DIRECTORIES include src
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_qs_lib(
    SOURCES ${_src}
    ENABLE_SHARED
    QT_LIBRARIES Core
    QT_PRIVATE_INCLUDES Core
    LINKS QsUtils
    INCLUDES include src
    WIN32_FILE_DESC "QSynthesis Common Utilities"
    WIN32_PRODUCT_NAME "QSynthesis.SVS"
)

target_link_libraries(${PROJECT_NAME} PRIVATE mrq)
