if(CK_BUILD_ALL_TOOLS)
    ck_add_subdirectories(ALL)
else()
    ck_add_subdirectories(${CK_BUILD_TOOLS})
endif()