project(coreplugin VERSION 0.1.0.0)

set(_res_dir Res)

# Get build information
set(_compiler_info "${CMAKE_CXX_COMPILER_ARCHITECTURE_ID} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
string(TIMESTAMP _time "%Y/%m/%d %H:%M:%S")
set(_git_branch "unknown")
set(_git_hash "unknown")

find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
        OUTPUT_VARIABLE _git_hash
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        WORKING_DIRECTORY
        ${REPOSITORY_ROOT_DIR}
    )

    execute_process(
        COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
        OUTPUT_VARIABLE _git_branch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        WORKING_DIRECTORY
        ${REPOSITORY_ROOT_DIR}
    )
else()
    message(STATUS "Git not found")
endif()

# Configure target
ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.hpp *.cpp *.cc
)

configure_plugin(_plugin "Sine Striker"
    MACRO_PREFIX CORE
    METADATA_IN plugin.json.in
    PLUGIN_NAME Core
)

ck_target_components(${_plugin}
    SOURCES ${_src} ${_res_dir}/core_res.qrc
    QT_LINKS Core Widgets
    LINKS QsFramework ExtensionSystem CkAppCore
    DEFINES_PRIVATE
        BUILD_COMPILER_INFO="${_compiler_info}"
        BUILD_YEAR="${ROOT_CURRENT_YEAR}"
        BUILD_DATETIME="${_time}"
        BUILD_GIT_COMMIT="${_git_hash}"
        BUILD_GIT_BRANCH="${_git_branch}"
    AUTO_INCLUDE_CURRENT
)

ck_add_translations(${_plugin}_translations
    PREFIX Core
    TARGETS ${_plugin}
    LOCALES zh_CN zh_HK ja_JP
    TS_DIR Res/translations
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/Res/translations
)

add_dependencies(${_plugin} ${_plugin}_translations_lrelease)

ck_add_attaches(${_plugin}
    SRC Res/themes DEST .
    SRC ${CMAKE_CURRENT_BINARY_DIR}/Res/translations DEST .
)

ck_install_headers(.)