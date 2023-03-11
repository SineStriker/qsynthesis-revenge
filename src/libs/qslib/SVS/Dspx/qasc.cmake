ck_add_files(_json_headers
    DIRS ${CMAKE_CURRENT_LIST_DIR}
    PATTERNS *.h *.hpp
)

qas_wrap_cpp(_qasc_src ${_json_headers} TARGET ${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${_qasc_src})