project(DsHost LANGUAGES CXX VERSION ${SUB_VERSION})

set(_res_dir Res)
get_subdirs(_dirs EXCLUDE ${_res_dir})

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "DiffScope.Intern"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} ${_res_dir}/res.qrc
    QT_LIBRARIES Widgets
    LINKS DsGui
    INCLUDES ${_dirs}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    APP_NAME="${SUB_APP_NAME}"
    APP_VERSION="${SUB_VERSION_SHORT}"
    APP_DESCRIPTION="${SUB_DESCRIPTION}"
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION ${_res_dir}/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/DsHost.res.json
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/translations/*.qm
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/DsHost.res/translations
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/themes/*.json
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/DsHost.res/themes
)