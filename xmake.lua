set_xmakever("2.6.0")

-- Pre-defined Variables
MAIN_TARGET = "LabelVoice"
APP_VERSION_VERBOSE = "0.0.1"
APP_DEPLOY = "false"
APP_LIB_DIR = "lib"
APP_PLUGINS_DIR = "plugins"

-- Project
set_project(MAIN_TARGET)
set_version(APP_VERSION_VERBOSE)

includes(src)