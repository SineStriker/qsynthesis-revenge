project(qzlib
    VERSION 0.0.0.1
    LANGUAGES CXX
)

add_files(_src
    CURRENT
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsUtils
    INCLUDE_CURRENT
    WIN32_AUTHOR_NAME "Sine Striker"
    WIN32_FILE_DESC "QSynthesis Plugin for File Compression"
)

target_link_libraries(${PROJECT_NAME} PRIVATE QuaZip::QuaZip)

settle_plugin(${PROJECT_NAME} compressengines)
