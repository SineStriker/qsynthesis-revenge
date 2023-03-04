project(QsFramework
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

get_subdirs(_dirs)
set(_extra_includes)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

ck_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.Framework"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src}
    QT_LIBRARIES Widgets
    QT_PRIVATE_INCLUDES Core Widgets
    LINKS QMWidgets QsMedia QsSVS
    LINKS_PRIVATE
    INCLUDES ${_dirs} ${_extra_includes}
)

# target_create_translations(${PROJECT_NAME} SOURCES ${_src}
#     LOCALES zh_CN zh_HK ja_JP
#     DESTINATION ${_res_dir}/translations
#     CREATE_ONCE
# )

# Copy resources
# ck_add_res(${PROJECT_NAME}
#     FILES ${_res_dir}/QsFramework.res.json
#     ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}
# )
# ck_add_res(${PROJECT_NAME}
#     FILES ${_res_dir}/translations/*.qm
#     ABSOLUTE_PATH ${SHARE_OUTPUT_DIR}/QsFramework.res/translations
# )