add_subdirectory(common)

# Copy doc
ck_add_shared_files(
    SKIP_BUILD
    SRC doc DEST .
)