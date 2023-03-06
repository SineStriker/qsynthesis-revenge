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

add_subdirectory(SVS)
add_subdirectory(Media)
add_subdirectory(Framework)
add_subdirectory(Plugins)