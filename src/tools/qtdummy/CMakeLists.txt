project(qtdummy VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

ck_add_files(_src ALLDIRS PATTERNS *.h *.c *.cpp)

ck_add_executable(${PROJECT_NAME})

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core Gui Widgets Network Concurrent
)

if(TRUE)
    ck_collect_targets(_all_targets DIR ${ROOT_SOURCE_DIR}/libs SHARED)

    set(_targets)

    foreach(_target ${_all_targets})
        if(_target MATCHES "Qs.*|QM.*|Ck.*")
            list(APPEND _targets ${_target})
        endif()
    endforeach()

    ck_add_translations(${PROJECT_NAME}_translations
        PREFIX qtdummy
        TARGETS ${_targets}
        LOCALES zh_CN zh_HK ja_JP
        TS_DIR Res/translations
        QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
    )

    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_translations_lrelease)

    ck_add_attaches(${PROJECT_NAME}
        SRC Res/conf/${CHORUSKIT_PLATFORM_SHORT}/qtmedium.conf.json DEST ${CHORUSKIT_RUNTIME_OUTPUT_DIR}
        SRC Res/themes DEST ${CHORUSKIT_SHARE_OUTPUT_DIR}/qtdummy
        SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST ${CHORUSKIT_SHARE_OUTPUT_DIR}/qtdummy
    )

    ck_add_attaches(${PROJECT_NAME}
        INSTALL_ONLY
        SRC Res/conf/${CHORUSKIT_PLATFORM_SHORT}/qt.conf DEST ${CHORUSKIT_QT_CONF_OUTPUT_DIR}
    )

    if(WIN32 OR APPLE)
        ck_add_deploy_qt_target(qtdummy_deploy TARGET ${PROJECT_NAME})
    endif()
endif()