if(APPLE)
    set(_project_lang C CXX OBJC)
else()
    set(_project_lang C CXX)
endif()

project(tst_DarkThemeTest VERSION 0.0.0.0 LANGUAGES ${_project_lang})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


if(APPLE)
    ck_add_files(_src
            CURRENT
            DIRS platforms/mac
            PATTERNS *.h *.cpp *.mm  # also include objc sources
    )
elseif(WIN32)
    ck_add_files(_src
            CURRENT
            DIRS platforms/win
            PATTERNS *.h *.cpp
    )
else()
    ck_add_files(_src
            CURRENT
            DIRS platforms/default
            PATTERNS *.h *.cpp
    )
endif()

ck_add_test(${PROJECT_NAME})

ck_target_components(${PROJECT_NAME}
        INCLUDE_CURRENT_PRIVATE
        SOURCES ${_src}
        QT_LINKS Core Widgets
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
endif()
