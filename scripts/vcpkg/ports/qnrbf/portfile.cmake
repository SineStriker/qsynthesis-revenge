vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/QNrbf
    REF 0.0.1.5
    SHA512 3f4f00e46508f67fe109cb1220c89665d12fc0726feac5dc460d673ad7efef1fd1c9192cf797d6b4ec074d69ec62bfb713ff9cd83c0808c4dcb7cf3c936a715e
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