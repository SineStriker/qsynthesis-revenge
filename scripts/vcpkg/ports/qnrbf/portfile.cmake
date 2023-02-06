vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/QNrbf
    REF 0.0.1.4
    SHA512 11aeb636c2890df603021dfd299afb7a15f88874cddb05ca8030b2a1a7187a9dd8ef2a4b2d6d07c372d497377f031a3aec19f2f441e824bdf5c49d60cfdf3c6c
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DQNRBF_BUILD_NRBF_ONLY=ON
        -DQNRBF_INSTALL=ON
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME QNrbf CONFIG_PATH lib/cmake/${PORT})
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)