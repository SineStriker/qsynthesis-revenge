project(qtdummy VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

ck_add_files(_src ALLDIRS PATTERNS *.h *.c *.cpp)
add_executable(${PROJECT_NAME})
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

    ck_add_translations(qtdummy_translations
        PREFIX qtdummy
        TARGETS ${_targets}
        LOCALES zh_CN zh_HK ja_JP
        TS_DIR Res/translations
        QM_DIR ${ROOT_SHARE_OUTPUT_DIRECTORY}/qtdummy/qtdummy.res/translations
        QM_TARGET ${PROJECT_NAME}
    )

    ck_add_attaches(${PROJECT_NAME}
        SRC Res/qtdummy.res.json DEST ${ROOT_SHARE_OUTPUT_DIRECTORY}/qtdummy
        SRC Res/themes DEST ${ROOT_SHARE_OUTPUT_DIRECTORY}/qtdummy/qtdummy.res
    )

    if(WIN32 OR APPLE)
        ck_add_deploy_qt_target(qtdummy_deploy TARGET ${PROJECT_NAME})

        # if(CMAKE_BUILD_TYPE MATCHES "Release|Rel")
        # string(TOLOWER ${APP_TOOLSET_NAME} TOOLSET_NAME_LOWER)
        # string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
        # string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)
        # set(_release_name ${TOOLSET_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})

        # install(DIRECTORY ${CONFIG_OUTPUT_DIRECTORY}
        # DESTINATION ${CMAKE_INSTALL_PREFIX}/${_release_name}
        # FILES_MATCHING PATTERN "*.h" ${CHORUSKIT_SHARED_LIBRARY_PATTERN}
        # )
        # endif()
    endif()
endif()