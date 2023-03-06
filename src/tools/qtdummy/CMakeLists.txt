project(qtdummy VERSION 0.0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ----------------------------------
# Add modules
# ----------------------------------
add_qt_module(_qt_libs Core Gui Widgets Network Concurrent)

# ----------------------------------
# Add target
# ----------------------------------
add_files(_src CURRENT_RECURSE PATTERNS *.h *.c *.cpp)
add_executable(${PROJECT_NAME} ${_src})

# ----------------------------------
# Target...
# ----------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE ${_qt_libs})

# ----------------------------------
# Add translations for libs
# ----------------------------------
if(TRUE)
    get_all_targets_recursive(_all_targets ${ROOT_SOURCE_DIR}/libs)

    set(_all_src)

    foreach(_target ${_all_targets})
        if(_target MATCHES "Qs.*|QM.*|NativeWindow|CKAppLoad")
            get_target_property(_src ${_target} SOURCES)
            list(APPEND _all_src ${_src})
        endif()
    endforeach()

    list(FILTER _all_src INCLUDE REGEX ".+\\.(cpp|cc)")
    list(FILTER _all_src EXCLUDE REGEX "(qasc|moc)_.+")

    target_create_translations(${PROJECT_NAME} SOURCES ${_all_src}
        LOCALES zh_CN zh_HK ja_JP
        DESTINATION Res/translations
        QM_DIR ${SHARE_OUTPUT_DIRECTORY}/qtdummy/translations
        CREATE_ONCE POST_BUILD
    )

    ck_add_res(${PROJECT_NAME}
        FILES Res/qtdummy.res.json
        ABSOLUTE_PATH ${SHARE_OUTPUT_DIRECTORY}/qtdummy
    )
endif()