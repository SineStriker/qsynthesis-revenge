vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/QNrbf
    REF 0.0.1.5
    SHA512 9f46e528309a1772a030c8e78bcf203c46bb5d10412cf098b48c7de03361933701b6ec76d1855db1db4c111ee1861cf5eeecc63ab73640606f727c3810821243
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
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)