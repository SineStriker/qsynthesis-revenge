project(CkIKg2p) # ChorusKit IceKyrin G2P

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.cpp
)

ck_add_library(${PROJECT_NAME} AUTOGEN)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core
    AUTO_INCLUDE_CURRENT
)