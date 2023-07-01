project(tst_ASIOTest VERSION 0.0.0.0)

if(NOT ASIOSDK_DIR)
    message("ASIOSDK_DIR not specified. ASIOTest will be skipped.")
else()
    ck_add_files(_src CURRENT PATTERNS *.h *.cpp)

    ck_add_test(${PROJECT_NAME} AUTOGEN)
    file(GLOB_RECURSE _asiosdk_src_common ${ASIOSDK_DIR}/common/*.cpp)
    list(FILTER _asiosdk_src_common EXCLUDE REGEX "asiodrvr")
    file(GLOB_RECURSE _asiosdk_src_host ${ASIOSDK_DIR}/host/*.cpp)
    list(FILTER _asiosdk_src_host EXCLUDE REGEX "hostsample")
    list(FILTER _asiosdk_src_host EXCLUDE REGEX "asiolist")
    add_library(ASIO_SDK STATIC ${_asiosdk_src_common} ${_asiosdk_src_host})
    target_include_directories(ASIO_SDK PUBLIC
        ${ASIOSDK_DIR}/common
        ${ASIOSDK_DIR}/driver/asiosample
        ${ASIOSDK_DIR}/host/pc
        ${ASIOSDK_DIR}/host
    )

    ck_target_components(${PROJECT_NAME}
        INCLUDE_CURRENT_PRIVATE
        SOURCES ${_src} Res/ASIOTest_res.qrc
        LINKS ASIO_SDK "winmm.lib"
        QT_LINKS Core Widgets
    )
endif()