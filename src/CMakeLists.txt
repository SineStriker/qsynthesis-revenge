cmake_minimum_required(VERSION 3.17)

project(QSynthesis LANGUAGES CXX)

# CPP Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Application Information
set(APP_DEPLOY OFF)
set(APP_VERSION 0.2.12)

# Determining Current System
set(SYS_WIN OFF)
set(SYS_MAC OFF)
set(SYS_LINUX OFF)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(SYS_WIN ON)
    message(STATUS "[INFO] Current System is Windows")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SYS_LINUX ON)
    message(STATUS "[INFO] Current System is Linux")
elseif (MACOS)
    set(SYS_MAC ON)
    message(STATUS "[INFO] Current System is Macintosh")
else ()
    message(FATAL_ERROR "Unsupported System !!!")
endif ()

# Libraries
add_subdirectory(FramelessHelper)

# Application
add_subdirectory(QSynthesis)
