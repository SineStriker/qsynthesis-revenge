project(LabelVoice
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

set(INNER_PROJECT ${PROJECT_NAME})

set(INNER_VERSION ${PROJECT_VERSION})

add_subdirectory(Core)

add_subdirectory(Plugin)

add_subdirectory(Staging)

add_subdirectory(WinMain)
