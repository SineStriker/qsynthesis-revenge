vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO wangwenx190/framelesshelper
    REF 2.3.0
    SHA512 a5d557362f63c9e50db85e4fa69a4e124a18e00216cf3485ab494eed9ed7d38cb1ea3f6d5f53a10348dd5dd5ec78f6b25a7de49f2752ba3e2da0fa47a419eeab
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DFRAMELESSHELPER_BUILD_STATIC=OFF
        -DFRAMELESSHELPER_BUILD_WIDGETS=ON
        -DFRAMELESSHELPER_BUILD_QUICK=OFF
        -DFRAMELESSHELPER_BUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/FramelessHelper)
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)