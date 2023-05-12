project(Aggregation VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

configure_qtc_library(${PROJECT_NAME} AGGREGATION_LIBRARY)

ck_target_components(${PROJECT_NAME}
    AUTO_INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS Core
)