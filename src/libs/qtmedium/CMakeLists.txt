project(QtMedium
    LANGUAGES CXX
    VERSION 0.2.18.1
)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(SUB_AUTHOR_NAME)
set(SUB_FILE_DESC)

function(configure_library)
    ck_add_library(${PROJECT_NAME} SHARED AUTOGEN
        DESCRIPTION "Qt With Advanced Implementations"
        VENDOR "QSynthesis Team"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_TRANSLATE TRUE
    )
endfunction()

add_subdirectory(CoreLib)
add_subdirectory(Widgets)

ck_install_headers(.)