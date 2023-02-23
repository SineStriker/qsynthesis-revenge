project(Utils VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_files(_src
    CURRENT
    DIRECTORIES mimetypes theme tooltip
    PATTERNS *.h *.cpp *.ui *.qrc
)

if(WIN32)
    list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.(h|cpp|hpp|cc)")
    list(FILTER _src EXCLUDE REGEX "unixutils.*")
elseif(APPLE)
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.(h|cpp|hpp|cc)")
    list(FILTER _src EXCLUDE REGEX "winutils.*")
    list(APPEND _src fileutils_mac.mm)
else()
    list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.(h|cpp|hpp|cc)")
    list(FILTER _src EXCLUDE REGEX "winutils.*")
endif()

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
    QT_LIBRARIES Core Widgets Network Concurrent
    LINKS
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QTCREATOR_UTILS_LIB)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE iphlpapi ws2_32 user32 shell32)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE "-framework Foundation")
endif()