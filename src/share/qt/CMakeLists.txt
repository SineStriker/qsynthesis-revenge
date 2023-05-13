project(qt_share)

# Add translations for libraries
file(GLOB _ts_files translations/*.ts)

ck_add_translations(${PROJECT_NAME}_translations
    TS_FILES ${_ts_files}
    QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/translations
)

# Copy files
ck_add_shared_files(
    SRC ${CMAKE_CURRENT_BINARY_DIR}/translations DEST ChorusKit
)