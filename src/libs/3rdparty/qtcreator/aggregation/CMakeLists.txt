project(Aggregation VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core
)

configure_qtc_library(${PROJECT_NAME})