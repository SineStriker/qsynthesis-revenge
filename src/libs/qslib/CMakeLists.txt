project(QsLib
    LANGUAGES CXX
    VERSION 0.2.18.1
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(SUB_AUTHOR_NAME "QSynthesis Team")
set(SUB_FILE_DESC "QSynthesis Library")

add_subdirectory(Base)
add_subdirectory(SVS)
add_subdirectory(Media)
add_subdirectory(Framework)
add_subdirectory(Plugins)

# Add translations
if(TRUE)
    get_all_targets(_all_targets)

    set(_all_src)

    foreach(_target ${_all_targets})
        if(_target MATCHES "Qs.*")
            get_target_property(_src ${_target} SOURCES)
            list(APPEND _all_src ${_src})
        endif()
    endforeach()

    list(FILTER _all_src INCLUDE REGEX ".+\\.(c|cpp|cc)")
    list(FILTER _all_src EXCLUDE REGEX "(qasc|moc)_.+")

    list(APPEND _all_src "../QtMedium/CoreLib/Global/QMSystem.cpp")
    list(APPEND _all_src "../choruskit/appload/main.cpp")

    target_create_translations(QsLib SOURCES ${_all_src}
        LOCALES zh_CN zh_HK ja_JP
        DESTINATION Res/translations
        CREATE_ONCE
    )

    add_custom_target(QsLib_res_utility ALL DEPENDS ${_all_src})

    ck_add_res(QsLib_res_utility
        FILES Res/QsLib.res.json
        ABSOLUTE_PATH ${SHARE_OUTPUT_DIRECTORY}/QsLib
    )

    ck_add_res(QsLib_res_utility
        FILES Res/translations/*.qm
        ABSOLUTE_PATH ${SHARE_OUTPUT_DIRECTORY}/QsLib/translations
    )
endif()