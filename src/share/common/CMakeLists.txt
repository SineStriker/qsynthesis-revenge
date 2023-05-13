project(common)

# Add translations for libraries
ck_collect_targets(_all_targets DIR ${CK_CMAKE_SOURCE_DIR}/libs SHARED)

set(_targets)
set(_qtc_targets)

foreach(_target ${_all_targets})
    get_target_property(_translate ${_target} QT_TRANSLATE)

    if(NOT _translate)
        continue()
    endif()

    get_target_property(_vendor ${_target} VENDOR)

    if(_vendor)
        list(APPEND _qtc_targets ${_target})
        continue()
    endif()

    list(APPEND _targets ${_target})
endforeach()

set_target_properties(ChorusKit_Metadata PROPERTIES DEPLOY_TARGETS "${_targets}")

# ChorusKit
ck_add_translations(${PROJECT_NAME}_translations
    PREFIX common
    TARGETS ${_targets}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/translations
)

# Qt Creator
ck_add_translations(qtcreator_translations
    PREFIX qtcreator
    TARGETS ${_qtc_targets}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/translations
)

# Copy files
ck_add_shared_files(
    SRC themes DEST ChorusKit
    SRC ${CMAKE_CURRENT_BINARY_DIR}/translations DEST ChorusKit
    SRC dict DEST ChorusKit/g2p
    SRC conf/${CK_PLATFORM_LOWER}/qtmedium.conf.json DEST ${CK_QT_CONF_OUTPUT_DIR}
)

ck_add_shared_files(
    SKIP_BUILD
    SRC conf/${CK_PLATFORM_LOWER}/qt.conf DEST ${CK_QT_CONF_OUTPUT_DIR}
)