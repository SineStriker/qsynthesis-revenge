project(DsCore LANGUAGES CXX VERSION ${SUB_VERSION})

get_subdirs(_dirs)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffScope.Core"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    LINKS QsSVS
    INCLUDES ${_dirs}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    APP_SHARE_DIR="${APP_SHARE_DIR}"
    APP_PLUGINS_DIR="${APP_PLUGINS_DIR}"
    APP_LOCALES_DIR="${APP_LOCALES_DIR}"
    APP_TOOLS_DIR="${APP_TOOLS_DIR}"
    APP_EXT_DIR="${APP_EXT_DIR}"
    APP_PUBLIC_PLUGINS_DIR="${APP_EXT_DIR}/${APP_PUBLIC_PLUGINS_DIR}"
    APP_PRIVATE_PLUGINS_DIR="${SUB_PLUGIN_DIR}"
)

# ----------------------------------
# Json serializations
# ----------------------------------
add_files(_json_headers
    DIRECTORIES Model
    PATTERNS *.h *.hpp
)

qas_wrap_cpp(_qasc_src ${_json_headers} TARGET ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${_qasc_src})
target_include_directories(${PROJECT_NAME} PUBLIC ${QASTOOL_INCLUDE_DIRS})

message(${QASTOOL_INCLUDE_DIRS})