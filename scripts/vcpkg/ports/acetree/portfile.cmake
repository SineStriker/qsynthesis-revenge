vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/AceTreeModel
    REF 0.0.1.1
    SHA512 7fad6a79067da0a7a4445585f370023a6456f6b24edaf0b56ae53216b851412d49946b6b11089b6bb98062b540805be9b47eb796b179ae65a966b98fc0fae678
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DACETREE_BUILD_TESTS=OFF
        -DACETREE_BUILD_EXAMPLES=OFF
        -DACETREE_BUILD_TOOLS=OFF
        -DACETREE_ENABLE_DEBUG=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME acetree CONFIG_PATH lib/cmake/${PORT})
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)