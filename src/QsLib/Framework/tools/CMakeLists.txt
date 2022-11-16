set(QAPPLICATION_CLASS QApplication)

# Include
add_subdirectory(SingleApplication)

# Settle
set_target_properties(SingleApplication PROPERTIES
    TC_TARGET_TYPE LIBRARY
    TC_LIBRARY_TYPE Tool
    TC_QT_BINARY true
)
