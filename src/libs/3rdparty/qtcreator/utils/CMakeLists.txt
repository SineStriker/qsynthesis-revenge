project(Utils VERSION ${QT_CREATOR_VERSION} LANGUAGES CXX)

ck_add_files(_src CURRENT PATTERNS *.h *.cpp *.ui *.qrc DIRS mimetypes theme tooltip FILTER_PLATFORM)

if(WIN32)
    list(FILTER _src EXCLUDE REGEX "unixutils.*")
    set(_platform_libs iphlpapi ws2_32 user32 shell32)
else()
    list(FILTER _src EXCLUDE REGEX "winutils.*")
endif()

if(APPLE)
    list(APPEND _src fileutils_mac.mm)
    set(_platform_link_flags "-framework Foundation")
endif()

if(QT_CREATOR_NO_FILENAME_LIST)
    set(_defines QT_CREATOR_NO_FILENAME_LIST)
endif()

set(_qt_libs Core Widgets Network Concurrent)

if(QT_CREATOR_ENABLE_QML)
    list(APPEND _src unused_src/templateengine.h unused_src/templateengine.cpp)
    list(APPEND _qt_libs Qml)
endif()

configure_qtc_library(${PROJECT_NAME} LIBRARY_MACRO QTCREATOR_UTILS_LIB)

ck_target_components(${PROJECT_NAME}
    AUTO_INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LINKS ${_qt_libs}
    DEFINES_PRIVATE ${_defines}
    CCFLAGS ${_platform_flags}
    LINKS ${_platform_link_flags}
    LINKS_PRIVATE ${_platform_libs}
)
