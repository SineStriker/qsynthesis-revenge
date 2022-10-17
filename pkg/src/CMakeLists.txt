add_subdirectory(quazip)

add_subdirectory(SDL2)

add_subdirectory(FFmpeg)

add_custom_target(install_all
    DEPENDS ffmpeg_install
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
)