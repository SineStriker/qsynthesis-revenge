if (APPLE)
#    add_link_options("-lcrypto")
endif()

add_compile_definitions(${PROJECT_NAME} PRIVATE CALIPER)