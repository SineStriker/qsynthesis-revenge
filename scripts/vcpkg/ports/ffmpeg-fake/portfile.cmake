set(LIB_PREFIX "https://github.com/SineStriker/ffmpeg-fake/releases/download")
set(LIB_VERSION 0.0.4)

if(WIN32)
    set(FILE_NAME ffmpeg-fake-windows-amd64-${LIB_VERSION}.zip)
    set(FILE_CHECK_SUM 8c607a3530fbe8402bc5aab51d350e54c24324d6480dab92ae2fbfd070c4718c30a523d971fd4d6c6f99281a8abfeea0650b4fb1392ac283a546a45d07263425)
elseif(APPLE)
    if(VCPKG_OSX_ARCHITECTURES STREQUAL arm64)
        set(FILE_NAME ffmpeg-fake-mac-arm64-${LIB_VERSION}.zip)
        set(FILE_CHECK_SUM 0)
    else()
        set(FILE_NAME ffmpeg-fake-mac-amd64-${LIB_VERSION}.zip)
        set(FILE_CHECK_SUM 0)
    endif()
else()
    set(FILE_NAME ffmpeg-fake-linux-amd64-${LIB_VERSION}.zip)
    set(FILE_CHECK_SUM 0)
endif()

vcpkg_download_distfile(ARCHIVE
    URLS ${LIB_PREFIX}/${LIB_VERSION}/${FILE_NAME}
    FILENAME ${FILE_NAME}
    SHA512 ${FILE_CHECK_SUM}
)

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"

    # NO_REMOVE_ONE_LEVEL
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
    -DCMAKE_INSTALL_INCLUDEDIR=${CURRENT_PACKAGES_DIR}/include
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/ffmpeg-fake)
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)
