project(mrq LANGUAGES C VERSION 0.2.0.0)

ck_add_library(${PROJECT_NAME})

ck_target_components(${PROJECT_NAME}
    SOURCES mrq.h mrq.c
    AUTO_INCLUDE_CURRENT
)

ck_install_headers(.)