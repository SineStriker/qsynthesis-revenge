# The macro works same as "option"
macro(option_ex _name _val)
    if (NOT DEFINED ${_name})
        set(${_name} ${_val})
    endif()
endmacro()

# Project Configurations
option_ex(MAIN_TARGET               LabelVoice)
option_ex(APP_VERSION_VERBOSE       0.0.1.3)
option_ex(APP_DEPLOY                off)
option_ex(APP_LIB_DIR               lib)
option_ex(APP_PLUGINS_DIR           plugins)

option_ex(BUILD_TEST                on)
option_ex(BUILD_SVS_PLUGINS_ONLY    off)

option_ex(DISABLE_UTA_PLUGINS       on)
option_ex(DISABLE_SVS_CONVERTERS    on)
option_ex(DISABLE_LV_PLUGINS        off)
