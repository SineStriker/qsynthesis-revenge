vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/AceTreeModel
    REF 0.0.1.1
    SHA512 fc64adf580c1b8b4b14d85b35dfc8fe6572bb1a791040db8350c2a74eb7c8bc5efdc92d1c39ac0039f1f74f0e8dfbeda9c741fcef0f0b7ee1b45146acda86793
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