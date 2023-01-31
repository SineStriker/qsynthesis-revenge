#include "DsStartupInfo.h"

#include <QApplication>

DsStartupInfo::DsStartupInfo() {
    qsLibPath = APP_LIB_DIR;
    appVersion = APP_VERSION;
    appDisplayName = APP_NAME;
    appDescription = APP_DESCRIPTION;
    parser.addPositionalArgument("files", "Project files to open.", "files");
}

DsStartupInfo::~DsStartupInfo() {
}
