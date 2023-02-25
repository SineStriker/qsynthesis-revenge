vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/QNrbf
    REF 0.0.1.4
    SHA512 57578c4ee4afb02d903ed634291145ecc4ff1158f1ca3630dd460f7707f9b6bf38f7c38206217614dc02f80111af82b6b6658c110c9d3027385f89696b85d32d
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