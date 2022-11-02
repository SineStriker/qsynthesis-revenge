project(LabelVoice
    LANGUAGES CXX
    VERSION ${APP_VERSION_VERBOSE}
)

add_subdirectory(Core)

add_subdirectory(Staging)

add_subdirectory(WinMain)
