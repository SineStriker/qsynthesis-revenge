# Open the project in src is suggested.

cmake_minimum_required(VERSION 3.17)

# Main Target
set(MAIN_TARGET LabelVoice)
set(APP_VERSION_VERBOSE 0.0.1.3)
set(APP_DEPLOY ON)

# Project
project(${MAIN_TARGET}
    LANGUAGES C CXX
    VERSION ${APP_VERSION_VERBOSE}
)

set(NOT_QT_CREATOR 1)

add_subdirectory(src)