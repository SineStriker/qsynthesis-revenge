project(QsSVS
    VERSION ${SUB_VERSION}
    LANGUAGES C CXX
)

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_library()

ck_target_components(${PROJECT_NAME}
    AUTO_INCLUDE_CURRENT INCLUDE_SUBDIRS_PRIVATE
    SOURCES ${_src}
    QT_LINKS Core
    QT_INCLUDES_PRIVATE Core
    LINKS QMCoreLib
    LINKS_PRIVATE mrq qastool::core
    AUTO_INCLUDE_DIRS Global
)

include(Dspx/qasc.cmake)