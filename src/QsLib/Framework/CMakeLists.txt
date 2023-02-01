project(QsFramework
    VERSION 0.2.17.0
    LANGUAGES CXX
)

set(_dirs Api Global Kernel Widgets)
set(_extra_includes
    Widgets/Custom
    Widgets/Extended
    Widgets/Metatypes
)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.Framework"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} Res/res.qrc
    QT_LIBRARIES Widgets Svg Network
    QT_PRIVATE_INCLUDES Widgets
    LINKS QsBase
    INCLUDES ${_dirs} ${_extra_includes}
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES Res/QsFramework.res.json
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES Res/translations/*.qm
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}/QsFramework.res/translations
)