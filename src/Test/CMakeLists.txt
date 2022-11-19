include(Template.cmake)

if(APP_DEPLOY)
    add_compile_definitions(TEST_LIB_DIR=".")
    add_compile_definitions(TEST_APP_DIR="..")
    add_compile_definitions(TEST_PLUGINS_DIR="../${APP_RES_DIR}/${APP_RES_BUILTIN_DIR}")
else()
    add_compile_definitions(TEST_LIB_DIR="${APP_LIB_DIR}")
    add_compile_definitions(TEST_APP_DIR=".")
    add_compile_definitions(TEST_PLUGINS_DIR="${APP_RES_DIR}/${APP_RES_BUILTIN_DIR}")
endif()

if(NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(PlayWindow)
    add_subdirectory(SpekTest)
    add_subdirectory(MrqTest)

    # add_subdirectory(PipeTest)
    # add_subdirectory(DecodeTest)
    # add_subdirectory(ZlibTest)
endif()

# add_subdirectory(UstxTest)
