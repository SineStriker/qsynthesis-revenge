project(QsLib
    LANGUAGES CXX
    VERSION 0.2.18.1
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
ck_get_short_version(SUB_VERSION_SHORT ${PROJECT_VERSION} 3)

set(SUB_AUTHOR_NAME "QSynthesis Team")
set(SUB_FILE_DESC "QSynthesis Library")

function(configure_library)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)
    ck_target_res(${PROJECT_NAME}
        DESCRIPTION ${SUB_FILE_DESC}
        COPYRIGHT ${SUB_AUTHOR_NAME}
    )
endfunction()

add_subdirectory(SVS)
add_subdirectory(Media)
add_subdirectory(Framework)
add_subdirectory(Plugins)