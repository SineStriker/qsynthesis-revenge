#include "LvStartupInfo.h"

LvStartupInfo::LvStartupInfo() {
    appVersion = APP_VERSION;
    appDisplayName = APP_NAME;
    appDescription = APP_DESCRIPTION;
    parser.addPositionalArgument("files", "Project files to open.", "files");
}

LvStartupInfo::~LvStartupInfo() {
}
