#include "QUtaStartupInfo.h"

QUtaStartupInfo::QUtaStartupInfo() {
    appVersion = APP_VERSION;
    appDisplayName = APP_NAME;
    appDescription = APP_DESCRIPTION;
    parser.addPositionalArgument("files", "Project files to open.", "files");
}

QUtaStartupInfo::~QUtaStartupInfo() {
}
