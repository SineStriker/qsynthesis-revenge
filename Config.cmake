# The macro works same as "option"
macro(option_ex _name)
    if(NOT DEFINED ${_name})
        set(${_name} ${ARGN})
    endif()
endmacro()

# Project Configurations
option_ex(APP_TOOLSET_NAME          ChorusKit)
option_ex(APP_TOOLSET_LIST          LabelVoice QSynthesis)
option_ex(APP_VERSION_VERBOSE       0.0.1.5)
option_ex(APP_DEPLOY                off)
option_ex(APP_LIB_DIR               lib)
option_ex(APP_PLUGINS_DIR           plugins) # Do not change
option_ex(APP_TOOLS_DIR             tools)
option_ex(APP_LOCALES_DIR           translations) # Do not change
option_ex(APP_RES_DIR               resources)
option_ex(APP_RES_BUILTIN_DIR       modules)
option_ex(APP_RES_PLUGINS_DIR       plugins)

option_ex(BUILD_TEST                on)
option_ex(BUILD_SVS_PLUGINS_ONLY    off)

option_ex(ENABLE_SVS_CONVERTERS     on)
option_ex(ENABLE_APP_PLUGINS        on)

option_ex(CONFIG_WIN32_DEBUG        on)
