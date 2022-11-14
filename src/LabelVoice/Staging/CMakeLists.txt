project(LvStaging
    LANGUAGES CXX
    VERSION ${INNER_VERSION}
)

add_files(_src
    CURRENT
    DIRECTORIES Kernel Managers Windows
    PATTERNS *.h *.hpp *.cpp *.cc
)

lvmod_configure_library(
    SOURCES ${_src} Resources/res.qrc
    ENABLE_SHARED
    QT_LIBRARIES Widgets Svg
    QT_PRIVATE_INCLUDES Gui Widgets
    LINKS LvCore LvElem
    INCLUDE_CURRENT
    WIN32_FILE_DESC "LabelVoice Main Module"
    WIN32_PRODUCT_NAME "LabelVoice.Staging"
)
