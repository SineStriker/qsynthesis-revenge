vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/AceTreeModel
    REF 0.0.1.1
    SHA512 e4914aa94692640e806584d45f23a08e435ba157cb60ee56dc5708550e5e781d76a04d4a94815fb858126f2029dca9b6b5c654ddf9790c8e3239b6811085df89
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