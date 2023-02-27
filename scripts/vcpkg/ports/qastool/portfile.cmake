vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SineStriker/qt-json-autogen
    REF 0.0.2.1
    SHA512 a30248e0aaf539f78f6baf4dde768cd9563bab0b58e8a12c47b34e52ded8f959598c7d5193249af4194a258de39b9f0a7f9fbf361e8e6a1df156df374159404f
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DQAS_BUILD_EXAMPLES=OFF
        -DQAS_BUILD_MOC_EXE=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(PACKAGE_NAME qastool
    CONFIG_PATH lib/cmake/${PORT}
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/lib")
file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/debug")
file(COPY "${CURRENT_PACKAGES_DIR}/tools" DESTINATION "${CURRENT_PACKAGES_DIR}/debug")

vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}/" RENAME copyright)