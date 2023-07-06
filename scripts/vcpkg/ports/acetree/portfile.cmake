vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/AceTreeModel
    REF 0.0.1.1
    SHA512 08440b241476b1a57e162cc9b9dd8507d56b046e447736d36331b17e71c107a20c932e6827a55eea314b31000bffb2b551aa35425c62a503a93d53ac83a95e71
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