project(qBreakpad VERSION 0.0.2.1 LANGUAGES CXX)

find_package(unofficial-breakpad CONFIG REQUIRED)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)

include(../config.cmake)

add_files(_src CURRENT_RECURSE PATTERNS *.h *.c *.cpp)

# ----------------------------------
# Build shared as LGPL requires
# ----------------------------------
qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME "buzzySmile"
    FILE_DESC "Cross-platform Qt crash handler"
    PRODUCT_NAME "${PEOJECT_NAME}"
    COPYRIGHT_YEAR "2021"
    LIBRARY_TYPE 3rdparty
)

# ----------------------------------
# Add components
# ----------------------------------
target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core Network
    LINKS   unofficial::breakpad::libbreakpad
            unofficial::breakpad::libbreakpad_client
)