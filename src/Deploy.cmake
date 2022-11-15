include(${PROJECT_MODULES_DIR}/Deploy.cmake)

set(_app_plugins)
set(_qs_plugins)
set(_app_libs)
set(_qs_libs)
set(_other_libs)
set(_main_exes)
set(_other_exes)

set(_qt_binaries)

# get_property(importTargets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY IMPORTED_TARGETS)
# foreach(_target ${importTargets})
# message(STATUS ${_target})
# endforeach()

# Target Collector
get_all_targets(_all_targets)

foreach(_target ${_all_targets})
    get_target_property(_target_type ${_target} TC_TARGET_TYPE)
    get_target_property(_is_qt_bin ${_target} TC_QT_BINARY)

    if(${_target_type} STREQUAL PLUGIN)
        get_target_property(_plugin_type ${_target} TC_PLUGIN_TYPE)

        if(${_plugin_type} STREQUAL ${MAIN_TARGET})
            list(APPEND _app_plugins ${_target})
        elseif(${_plugin_type} STREQUAL QsLib)
            list(APPEND _qs_plugins ${_target})
        endif()

    elseif(${_target_type} STREQUAL LIBRARY)
        get_target_property(_lib_type ${_target} TC_LIBRARY_TYPE)

        if(${_lib_type} STREQUAL LabelVoice)
            list(APPEND _app_libs ${_target})
        elseif(${_lib_type} STREQUAL QSynthesis)
            list(APPEND _qs_libs ${_target})
        else()
            list(APPEND _other_libs ${_target})
        endif()

    elseif(${_target_type} STREQUAL EXECUTABLE)
        get_target_property(_exe_type ${_target} TC_EXECUTABLE_TYPE)

        if(${_exe_type} STREQUAL Main)
            list(APPEND _main_exes ${_target})
        else()
            list(APPEND _other_exes ${_target})
        endif()
    endif()

    if((NOT ${_is_qt_bin} STREQUAL "_qt_bin-NOTFOUND") AND ${_is_qt_bin})
        list(APPEND _qt_binaries ${_target})
    endif()
endforeach()

# Add deploy target
message(STATUS "[INFO] Deployment mode, virtual target is added")

if(NOT WIN32)
    message(FATAL_ERROR "Deployment mode is only implemented on Windows!")
endif()

string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
string(TOLOWER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_LOWER)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_ARCH_LOWER)

set(_release_name ${PROJECT_NAME_LOWER}-${SYSTEM_NAME_LOWER}-${SYSTEM_ARCH_LOWER}-${APP_VERSION_VERBOSE})

set(_deploy_dir ${PROJECT_RELEASE_DIR}/${_release_name})
set(_libs_dir ${_deploy_dir}/${APP_LIB_DIR})
set(_plugins_dir ${_deploy_dir}/${APP_PLUGINS_DIR})
set(_res_dir ${_deploy_dir}/${APP_RES_DIR})
set(_res_docs_dir ${_res_dir}/docs)
set(_res_builtin_dir ${_res_dir}/${APP_RES_BUILTIN_DIR})
set(_res_plugins_dir ${_res_dir}/${APP_RES_PLUGINS_DIR})

# Find Qt tools
get_filename_component(QT_BIN_DIRECTORY "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
find_program(QT_DEPLOY_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${QT_BIN_DIRECTORY}")

if(NOT EXISTS "${QT_DEPLOY_EXECUTABLE}")
    message("Cannot find the deployqt tool.")
    return()
endif()

if(NOT DEFINED QT_QMAKE_EXECUTABLE)
    get_target_property(QT_QMAKE_EXECUTABLE Qt::qmake IMPORTED_LOCATION)
endif()

if(NOT EXISTS "${QT_QMAKE_EXECUTABLE}")
    message("Cannot find the QMake executable.")
    return()
endif()

# Add deploy target
add_custom_target(deploy DEPENDS all)

# Make deploy dir
add_custom_command(
    TARGET deploy
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${_deploy_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_deploy_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_libs_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_plugins_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_res_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_res_docs_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_res_builtin_dir}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${_res_plugins_dir}
)

# Copy docs dir
get_filename_component(_license ${PROJECT_LICENSE_FILE} NAME)
add_custom_command(
    TARGET deploy
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_DOCUMENT_DIR} ${_res_docs_dir}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_LICENSE_FILE} ${_res_docs_dir}
    COMMAND ${CMAKE_COMMAND} -E rename ${_res_docs_dir}/${_license} ${_res_docs_dir}/license.txt
)

# Deploy lv plugins
foreach(_plugin ${_app_plugins})
    get_target_property(_category ${_plugin} TC_PLUGIN_CATEGORY)

    if(${_category} STREQUAL "_category-NOTFOUND")
        continue()
    endif()

    set(_category_dir ${_res_plugins_dir}/${_category})
    add_custom_command(
        TARGET deploy
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_category_dir}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${_plugin}> ${_category_dir}
    )
    unset(_category_dir)
endforeach()

# Deploy qs plugins
foreach(_plugin ${_qs_plugins})
    get_target_property(_category ${_plugin} TC_PLUGIN_CATEGORY)

    if(${_category} STREQUAL "_category-NOTFOUND")
        continue()
    endif()

    set(_category_dir ${_res_builtin_dir}/${_category})
    add_custom_command(
        TARGET deploy
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_category_dir}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${_plugin}> ${_category_dir}
    )
    unset(_category_dir)
endforeach()

# Deploy libraries
foreach(_lib ${_app_libs} ${_qs_libs} ${_other_libs})
    # Set output dir to APP_LIB_DIR
    if(WIN32)
        set_target_properties(
            ${_lib} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_LIB_DIR}
        )
    else()
        set_target_properties(
            ${_lib} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${APP_LIB_DIR}
        )
    endif()

    add_custom_command(
        TARGET deploy
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${_lib}> ${_libs_dir}
    )
endforeach()

# Deploy executables
foreach(_exe ${_main_exes} ${_other_exes})
    add_custom_command(
        TARGET deploy
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${_exe}> ${_deploy_dir}
    )
endforeach()

# Deploy imported libraries
if(WIN32)
    file(GLOB _dlls ${VCPKG_BINARY_DIR}/${VCPKG_BINARY_PAT})

    foreach(_dll ${_dlls})
        get_filename_component(_name ${_dll} NAME)
        add_custom_command(
            TARGET deploy
            COMMAND echo "Deploy ${_name}"
            COMMAND ${CMAKE_COMMAND} -E copy ${_dll} ${_libs_dir}
        )
    endforeach()
endif()

# Deploy Qt
foreach(_bin ${_qt_binaries})
    add_custom_command(
        TARGET deploy
        COMMAND echo "Deploy $<TARGET_FILE_NAME:${_bin}>"
        COMMAND ${QT_DEPLOY_EXECUTABLE}
        --libdir ${_libs_dir}
        --plugindir ${_plugins_dir}
        --no-translations
        --no-system-d3d-compiler
        --no-virtualkeyboard
        --no-compiler-runtime
        --no-opengl-sw
        --force
        --verbose 0
        $<TARGET_FILE:${_bin}>
    )
endforeach()