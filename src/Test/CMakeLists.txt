include(lvtest.cmake)

if (NOT BUILD_SVS_PLUGINS_ONLY)
    add_subdirectory(PlayWindow)
    add_subdirectory(SpekTest)
#    add_subdirectory(PipeTest)
#    add_subdirectory(DecodeTest)
#    add_subdirectory(ZlibTest)
endif()

#add_subdirectory(UstxTest)
