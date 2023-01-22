set(CURRENT_PLUGIN_CATEGORY windowfactories)
set(CURRENT_PLUGIN_DESC "QSynthesis Library Plugin")

if (NOT APPLE)
    add_subdirectory(NativeWindow)
endif()
