project(QSynthesis
    LANGUAGES CXX
    VERSION 0.2.18.4
)

include(Template.cmake)

# Set global variable for current project
set(SUB_PROJECT ${PROJECT_NAME})
set(SUB_VERSION ${PROJECT_VERSION})
set(SUB_VERSION_SHORT ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(SUB_DESCRIPTION "Remake of Vocal Synthesis UTAU.")
set(SUB_IDENTIFIER "com.SineStr.QSynthesis")

set(SUB_PLUGIN_DIR ${APP_RES_DIR}/qsynthesis)

# Add subdirectories
add_subdirectory(Core)
add_subdirectory(Element)
add_subdirectory(Staging)
add_subdirectory(Plugin)
add_subdirectory(WinMain)
