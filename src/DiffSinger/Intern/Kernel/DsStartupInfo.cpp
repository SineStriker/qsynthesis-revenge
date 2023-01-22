#include "DsStartupInfo.h"

DsStartupInfo::DsStartupInfo() {
    appVersion = APP_VERSION;
    appDisplayName = APP_NAME;
    appDescription = APP_DESCRIPTION;
    parser.addPositionalArgument("files", "Project files to open.", "files");
}

DsStartupInfo::~DsStartupInfo() {
}
