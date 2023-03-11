project(QMCoreLib
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_platform_libs)

if(APPLE)
    list(APPEND _platform_libs ObjC)
endif()

if(NOT WIN32)
    list(APPEND _platform_libs dl)
endif()

set(_extra_includes)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT INCLUDE_SUBDIRS
    SOURCES ${_src}
    QT_LINKS Core
    QT_INCLUDES_PRIVATE Core
    LINKS
    LINKS_PRIVATE ${_platform_libs}
    INCLUDES ${_extra_includes}
)