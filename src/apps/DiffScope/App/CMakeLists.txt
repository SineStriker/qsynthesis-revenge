project(${SUB_PROJECT}
    LANGUAGES CXX
    VERSION ${SUB_VERSION}
)

# ----------------------------------
# Add application lib
# ----------------------------------
set(_lib_main ${PROJECT_NAME}Main)

qs_add_library(${_lib_main}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffScope.Main"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${_lib_main}
    INCLUDE_CURRENT
    SOURCES main.cpp
    QT_LIBRARIES Core Widgets
    LINKS ExtensionSystem
    INCLUDES ${COMMON_INCLUDE_DIR} ${_dirs}
)

# ----------------------------------
# Add application exe
# ----------------------------------
qs_add_executable(${PROJECT_NAME} ${_lib_main}
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffScope"
    EXECUTABLE_TYPE Main
    ICO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/win32/app.ico
    ICNS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/mac/app.icns
    OUTPUT_NAME ${SUB_APP_NAME}
)