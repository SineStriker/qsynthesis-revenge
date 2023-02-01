project(QsIntegrate
    VERSION 0.0.1.0
    LANGUAGES CXX
)

set(_dirs Editor Global Kernel Managers Widgets)
set(_extra_includes)

add_files(_src
    DIRECTORIES ${_dirs}
    PATTERNS *.h *.hpp *.cpp *.cc
)

qs_add_library(${PROJECT_NAME}
    ENABLE_SHARED
    AUTHOR_NAME ${SUB_AUTHOR_NAME}
    FILE_DESC ${SUB_FILE_DESC}
    PRODUCT_NAME "QSynthesis.Library.Integrate"
    LIBRARY_TYPE ${SUB_PROJECT}
)

target_components(${PROJECT_NAME}
    INCLUDE_CURRENT
    SOURCES ${_src} Res/res.qrc
    QT_LIBRARIES Widgets Svg Network
    QT_PRIVATE_INCLUDES Widgets
    LINKS QsFramework QsMedia QsSVS
    INCLUDES ${_dirs} ${_extra_includes}
)

target_create_translations(${PROJECT_NAME} SOURCES ${_src}
    LOCALES zh_CN zh_HK ja_JP
    DESTINATION res/translations
    CREATE_ONCE
)

# Copy resources
qs_add_res(${PROJECT_NAME}
    FILES Res/QsIntegrate.res.json
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}
)
qs_add_res(${PROJECT_NAME}
    FILES Res/translations/*.qm
    ABSOLUTE_PATH ${RES_OUTPUT_DIR}/QsIntegrate.res/translations
)