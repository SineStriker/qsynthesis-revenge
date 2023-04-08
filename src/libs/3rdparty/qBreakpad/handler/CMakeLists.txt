project(qBreakpad VERSION 0.2.1.1 LANGUAGES CXX)

find_package(unofficial-breakpad CONFIG REQUIRED)

include(../config.cmake)

ck_add_files(_src ALLDIRS PATTERNS *.h *.cpp)

ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Network
    LINKS unofficial::breakpad::libbreakpad unofficial::breakpad::libbreakpad_client
    AUTO_INCLUDE_CURRENT
)

ck_target_res(${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "Cross-platform Qt crash handler"
    COPYRIGHT "Copyright buzzySmile 2021"
)