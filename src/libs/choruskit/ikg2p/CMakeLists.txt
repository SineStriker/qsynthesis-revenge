project(CkIKg2p) # ChorusKit IceKyrin G2P

ck_add_files(_src
    ALLDIRS
    PATTERNS *.h *.cpp
)

configure_library(
    AUTOGEN
    VENDOR "IceKyrin"
)

ck_target_components(${PROJECT_NAME}
    SOURCES ${_src}
    QT_LINKS Core
    AUTO_INCLUDE_CURRENT
)