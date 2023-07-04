vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/AceTreeModel
    REF 0.0.1.1
    SHA512 d2caadc1f5447ebd7843f7019fc9425e12c043e4a8f45a8e9e0a1f9b8da5cac113bf1b86731e97bbe10fd7e815bd992678e4ddf926aca3104ae0380d46b24510
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