project(QtMedium
    LANGUAGES CXX
    VERSION 0.2.18.1
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(SUB_AUTHOR_NAME "QSynthesis Team")
set(SUB_FILE_DESC "Qt With Advanced Implementations")

function(configure_library)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN)
    ck_target_res(${PROJECT_NAME}
        DESCRIPTION ${SUB_FILE_DESC}
        COPYRIGHT ${SUB_AUTHOR_NAME}
    )
endfunction()

add_subdirectory(CoreLib)
add_subdirectory(Widgets)