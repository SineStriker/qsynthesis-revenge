project(QMCore
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_platform_libs)

if(APPLE)
    list(APPEND _platform_libs "-framework CoreFoundation")
endif()

if(NOT WIN32)
    list(APPEND _platform_libs dl)
endif()

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core
    QT_INCLUDES_PRIVATE Core
    LINKS
    LINKS_PRIVATE ${_platform_libs}
    AUTO_INCLUDE_CURRENT
    AUTO_INCLUDE_DIRS Global Kernel Text Collections Serialization Tools ../..
    INCLUDE_PRIVATE ${CK_HEADERS_OUTPUT_PATH}
    INCLUDE_SUBDIRS_PRIVATE
)

ck_dir_skip_automoc(Collections Serialization Text Tools)