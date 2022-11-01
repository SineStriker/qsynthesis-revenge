set(LIB_PREFIX "https://github.com/SineStriker/ffmpeg-fake/releases/download")
set(LIB_VERSION 0.0.3)

if (WIN32)
    set(FILE_NAME ffmpeg-fake-windows-amd64-${LIB_VERSION}.zip)
    set(FILE_CHECK_SUM 43e1a793e72332663eacad2473a8ee5be64f6e55faa5de1ce658c99478ac1e4739ec00cdd21bf0eac9f32487b62225f5cc3b977ce110b8ee8b5c83c7d6d5e69e)
elseif (APPLE)
    set(FILE_NAME ffmpeg-fake-mac-amd64-${LIB_VERSION}.zip)
    set(FILE_CHECK_SUM 0)
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