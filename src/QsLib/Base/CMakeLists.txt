project(QsBase
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_res_dir Res)
get_subdirs(_dirs EXCLUDE ${_res_dir})
get_subdirs(_tools_dirs DIRECTORY Tools RELATIVE .)

if(APPLE)
    set(_platform_lib ObjC)
endif()

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.Base"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Core
    QT_PRIVATE_INCLUDES Core
    LINKS_PRIVATE SingleApplication ${_platform_lib}
    INCLUDES ${_dirs} ${_tools_dirs}
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION ${_res_dir}/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/QsBase.res.json
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES ${_res_dir}/translations/*.qm
    ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/QsBase.res/translations
)