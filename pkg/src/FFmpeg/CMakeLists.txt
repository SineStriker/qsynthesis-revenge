cmake_minimum_required(VERSION 3.17)

project(ffmpeg
    VERSION 0.0.1
)

# libavcodec provides implementation of a wider range of codecs.
# libavformat implements streaming protocols, container formats and basic I/O access.
# libavutil includes hashers, decompressors and miscellaneous utility functions.
# libavfilter provides means to alter decoded audio and video through a directed graph of connected filters.
# libavdevice provides an abstraction to access capture and playback devices.
# libswresample implements audio mixing and resampling routines.
# libswscale implements color conversion and scaling routines.

# Hint: build ffmpeg independently and copy the contents of result directory here.
# The dir structure should be like:
#
#   CMakeLists.txt
# + usr
#   + bin
#       *.dll/*.so
#       ...
#   + include
#     + libavcodec
#     + libavformat
#       ...
#   + lib
#       *.lib/*.a
#       ...
#   + share
#     + ffmpeg
#       ...

set(ffmpeg_dir ${CMAKE_CURRENT_SOURCE_DIR}/usr)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include/FFmpeg")
set(INSTALL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/cmake/FFmpeg")

add_custom_target(ffmpeg_install)

add_custom_command(TARGET ffmpeg_install
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ffmpeg_dir}/bin ${INSTALL_BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ffmpeg_dir}/lib ${INSTALL_LIB_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ffmpeg_dir}/include ${INSTALL_INC_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ffmpeg_dir}/share ${CMAKE_INSTALL_PREFIX}/share
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${INSTALL_CMAKE_DIR}
)
