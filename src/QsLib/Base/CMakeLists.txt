project(QsBase
    VERSION ${SUB_VERSION}
    LANGUAGES CXX
)

set(_dirs Api Global Kernel Serialization Tools)

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
    LINKS_PRIVATE SingleApplication
    INCLUDES ${_dirs}
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES Res/QsBase.res.json
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES Res/translations/*.qm
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}/QsBase.res/translations
)