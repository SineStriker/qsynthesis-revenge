project(Aggregation VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

add_files(_src CURRENT PATTERNS *.h *.cpp)

# ----------------------------------
# Build shared as LGPL requires
# ----------------------------------
configure_qtc_library(${PROJECT_NAME})

# ----------------------------------
# Add components
# ----------------------------------
target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS
)