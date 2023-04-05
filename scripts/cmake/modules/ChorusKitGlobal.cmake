include_guard(DIRECTORY)

set(CHORUSKIT_CMAKE_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CHOURSKIT_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR}/commands)

if(WIN32)
    set(CHORUSKIT_SHARED_LIBRARY_PATTERN "*.dll")
    set(CHORUSKIT_EXECUTABLE_RPATH "")
    set(CHORUSKIT_LIBRARY_RPATH "")
    set(CHORUSKIT_PLUGIN_RPATH "")
elseif(APPLE)
    set(CHORUSKIT_SHARED_LIBRARY_PATTERN "*.dylib")
    set(CHORUSKIT_EXECUTABLE_RPATH "@executable_path;@executable_path/../Frameworks")
    set(CHORUSKIT_LIBRARY_RPATH "@loader_path")
    set(CHORUSKIT_PLUGIN_RPATH "@loader_path;@loader_path/../../../Frameworks") # Plugins/_app/_plugin/xxx.dylib
else()
    set(CHORUSKIT_SHARED_LIBRARY_PATTERN "*.so*")
    set(CHORUSKIT_EXECUTABLE_RPATH "@ORIGIN/../lib")
    set(CHORUSKIT_LIBRARY_RPATH "@ORIGIN")
    set(CHORUSKIT_PLUGIN_RPATH "@ORIGIN;@ORIGIN/../../..") # lib/_app/plugins/_plugin/xxx.so
endif()